#include "glwindow.h"
#include "box.h"
#include <QDebug>
#include <QOpenGLFramebufferObject>

GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

GLWindow::~GLWindow()
{
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    QString glVstr=tr("ligntDemo (OpenGL Version : %1.%2)").arg(this->format().version().first).arg(this->format().version().second);
    this->setWindowTitle(glVstr);

    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
    gl->glClearColor(0, 0, 0, 1.0f);

    mVAO.create();
    mVAO.bind();

    mBox = new Box(this);
    initShaderProgram();
}

void GLWindow::resizeGL(int w, int h)
{
    mProjMatrix.setToIdentity();
    mProjMatrix.perspective(70.0f, GLfloat(w) / h, 0.001f, 10000.0f);
}

void GLWindow::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);

//    QOpenGLFramebufferObject fbo(1280, 720);
//    fbo.bind();

    mRenderShader->bind();

    QMatrix4x4 camera;
    camera.setToIdentity();
    camera.translate(0, -1, -3);
    camera.rotate(300, 0, 1, 0);
//    camera.rotate(10, 1, 0, 0);
//    camera.rotate(0, 0, 0, 1);

    QMatrix4x4 rotMat;
    rotMat.rotate(10, 1, 0, 0);
    camera = rotMat * camera;


    mRenderShader->setUniformValue(mMatrixLoc, camera);
    mRenderShader->setUniformValue(mProjLoc, mProjMatrix);
    mRenderShader->setUniformValue(mAmbientLoc, QVector4D(1.0, 1.0, 1.0, 1.0));     //环境光
    QMatrix4x4 normalMat;
    normalMat.setToIdentity();
    mRenderShader->setUniformValue(mNormalLoc, normalMat.normalMatrix());
    mRenderShader->setUniformValue(mLightPosLoc, QVector3D(0.4f, 1.0f, 0.6f));

    mBox->bind();

    f->glVertexAttribPointer(mPosAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    f->glVertexAttribPointer(mColAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(QVector3D)));
    f->glVertexAttribPointer(mNormalAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(QVector3D) + sizeof(QVector3D)));

    f->glEnableVertexAttribArray(mPosAttr);
    f->glEnableVertexAttribArray(mColAttr);
    f->glEnableVertexAttribArray(mNormalAttr);

    f->glDrawArrays(GL_TRIANGLES, 0, mBox->getVertexCount());

    f->glDisableVertexAttribArray(mNormalAttr);
    f->glDisableVertexAttribArray(mColAttr);
    f->glDisableVertexAttribArray(mPosAttr);
    mBox->release();

    mRenderShader->release();

//    fbo.release();
}

void GLWindow::closeEvent(QCloseEvent *event)
{
}

void GLWindow::initShaderProgram()
{
    mRenderShader = new QOpenGLShaderProgram(this);
    mRenderShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/scene.vert");
    mRenderShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/scene.frag");
    mRenderShader->link();

    mRenderShader->bind();
    mPosAttr = mRenderShader->attributeLocation("posAttr");
    mColAttr = mRenderShader->attributeLocation("colAttr");
    mNormalAttr = mRenderShader->attributeLocation("normalAttr");
    mMatrixLoc = mRenderShader->uniformLocation("matrix");
    mProjLoc = mRenderShader->uniformLocation("projMatrix");
    mNormalLoc = mRenderShader->uniformLocation("normalMatrix");
    mAmbientLoc = mRenderShader->uniformLocation("ambient");
    mLightPosLoc = mRenderShader->uniformLocation("lightPos");
    mRenderShader->release();
}
