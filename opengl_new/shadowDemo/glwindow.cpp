#include "glwindow.h"
#include "box.h"
#include <QDebug>
#include <QOpenGLFramebufferObject>
#include <QTimer>

GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    mRotAngle = 0;
    startTimer(1000 / 60);
}

GLWindow::~GLWindow()
{
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    QString glVstr=tr("(OpenGL Version : %1.%2)").arg(this->format().version().first).arg(this->format().version().second);
    this->setWindowTitle(glVstr);

    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
    gl->glClearColor(0, 0, 0, 1.0f);

    mVAO.create();
    mVAO.bind();

    mBox = new Box(this);
    initShaderProgram();

    depTextureId = 0;
    gl->glGenTextures(1, &depTextureId);
    gl->glBindTexture(GL_TEXTURE_2D, depTextureId);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1280, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl->glBindTexture(GL_TEXTURE_2D, 0);

    depFbo = 0;
    gl->glGenFramebuffers(1, &depFbo);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, depFbo);
    glDrawBuffer(GL_NONE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depTextureId, 0);
}

void GLWindow::resizeGL(int w, int h)
{
    mProjMatrix.setToIdentity();
    mProjMatrix.perspective(70.0f, GLfloat(w) / h, 0.001f, 10000.0f);
}

void GLWindow::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glBindFramebuffer(GL_FRAMEBUFFER, depFbo);
//    QOpenGLFramebufferObject fbo(1024, 1024);

//    fbo.bind();

    f->glClearDepthf(1.0f);
    f->glClear(GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_CULL_FACE);
    f->glEnable(GL_POLYGON_OFFSET_FILL);
    f->glPolygonOffset(2.0f, 4.0f);

    mShadowShader->bind();
    f->glBindTexture(GL_TEXTURE_2D, 0);

    QMatrix4x4 depCamera;
    depCamera.lookAt(QVector3D(10, 1, 0), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    QMatrix4x4 drotMat;
    drotMat.setToIdentity();
    drotMat.rotate(mRotAngle, 0, 1, 0);
    depCamera = depCamera * drotMat;

    QMatrix4x4 depModelMat;
    depModelMat.setToIdentity();

    mShadowShader->setUniformValue(mUDepModelMatrix, depModelMat);
    mShadowShader->setUniformValue(mUDepViewMatrix, depCamera);
    mShadowShader->setUniformValue(mUDepProjMatrix, mProjMatrix);

    mBox->bind();

    f->glVertexAttribPointer(mADepPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    f->glEnableVertexAttribArray(mADepPos);

    f->glDrawArrays(GL_TRIANGLES, 0, mBox->getVertexCount());

    f->glDisableVertexAttribArray(mADepPos);
    mBox->release();

    mShadowShader->release();

    f->glDisable(GL_POLYGON_OFFSET_FILL);

    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    fbo.release();

//    return;
    // render scene
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);

    f->glBindTexture(GL_TEXTURE_2D, depTextureId);
    mRenderShader->bind();

    QMatrix4x4 camera;
//    camera.lookAt(QVector3D(0, 1, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    camera.setToIdentity();
    camera.translate(0, -1, -5);
    QMatrix4x4 rotMat;
    rotMat.setToIdentity();
    rotMat.rotate(0, 0, 1, 0);
    camera = camera * rotMat;

    QMatrix4x4 modelMat;
    modelMat.setToIdentity();
//    modelMat.rotate(mRotAngle, 0, 1, 0);

    QMatrix4x4 biasMatrix(
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, 0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    biasMatrix.optimize();

    QMatrix4x4 normalMat;
    normalMat.setToIdentity();

    mRenderShader->setUniformValue(mUViewMatrix, camera);
    mRenderShader->setUniformValue(mUProjMatrix, mProjMatrix);
    mRenderShader->setUniformValue(mUModelMatrix, modelMat);
    mRenderShader->setUniformValue(mUBiasMatrix, biasMatrix);
    mRenderShader->setUniformValue(mUShadowMatrix, depCamera);
    mRenderShader->setUniformValue(mUNormalMatrix, normalMat.normalMatrix());
    mRenderShader->setUniformValue(mULightDirection, QVector3D(0.4f, 1.0f, 0.6f));
    mRenderShader->setUniformValue(mUShadowTexture, 0);

    mBox->bind();

    f->glVertexAttribPointer(mAVertex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    f->glVertexAttribPointer(mAColor, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(QVector3D)));
    f->glVertexAttribPointer(mANormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(QVector3D) + sizeof(QVector3D)));

    f->glEnableVertexAttribArray(mAVertex);
    f->glEnableVertexAttribArray(mAColor);
    f->glEnableVertexAttribArray(mANormal);

    f->glDrawArrays(GL_TRIANGLES, 0, mBox->getVertexCount());

    f->glDisableVertexAttribArray(mANormal);
    f->glDisableVertexAttribArray(mAColor);
    f->glDisableVertexAttribArray(mAVertex);
    mBox->release();

    f->glBindTexture(GL_TEXTURE_2D, 0);
    mRenderShader->release();

    mRotAngle += 1;
    if(mRotAngle > 360)
        mRotAngle = mRotAngle - 360;
}

void GLWindow::closeEvent(QCloseEvent *event)
{
}

void GLWindow::timerEvent(QTimerEvent *event)
{
    update();
}

void GLWindow::initShaderProgram()
{
    mRenderShader = new QOpenGLShaderProgram(this);
    mRenderShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/scene.vert");
    mRenderShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/scene.frag");
    mRenderShader->link();

    mRenderShader->bind();
    mAVertex = mRenderShader->attributeLocation("a_vertex");
    mAColor = mRenderShader->attributeLocation("a_color");
    mANormal = mRenderShader->attributeLocation("a_normal");
    mUProjMatrix = mRenderShader->uniformLocation("u_projMatrix");
    mUModelMatrix = mRenderShader->uniformLocation("u_modelMatrix");
    mUViewMatrix = mRenderShader->uniformLocation("u_viewMatrix");
    mUBiasMatrix = mRenderShader->uniformLocation("u_biasMatrix");
    mUShadowMatrix = mRenderShader->uniformLocation("u_shadowMatrix");
    mUNormalMatrix = mRenderShader->uniformLocation("u_normalMatrix");
    mULightDirection = mRenderShader->uniformLocation("u_lightDirection");
    mUShadowTexture = mRenderShader->uniformLocation("u_shadowTexture");
    mRenderShader->release();

    mShadowShader = new QOpenGLShaderProgram(this);
    mShadowShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/abs.vert");
    mShadowShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/abs.frag");
    mShadowShader->link();

    mShadowShader->bind();
    mADepPos = mShadowShader->attributeLocation("posAttr");
    mUDepModelMatrix = mShadowShader->uniformLocation("u_modelMatrix");
    mUDepViewMatrix = mShadowShader->uniformLocation("u_viewMatrix");
    mUDepProjMatrix = mShadowShader->uniformLocation("u_projMatrix");
    mShadowShader->release();
}
