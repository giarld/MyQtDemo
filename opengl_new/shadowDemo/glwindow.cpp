#include "glwindow.h"
#include "previewwidget.h"
#include "box.h"
#include <QDebug>
#include <QOpenGLFramebufferObject>

GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    mPreviewWidget = new PreviewWidget(0);
    mPreviewWidget->show();
    mPreviewWidget->setGeometry(0, 0, this->width(), this->height());
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

    mRenderShader->bind();

    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0, 0, -2);
    matrix.rotate(45, 0, 1, 0);
    matrix.rotate(45, 1, 0, 0);
    matrix.rotate(0, 0, 0, 1);
    mRenderShader->setUniformValue(mMatrixLoc, matrix);
    mRenderShader->setUniformValue(mProjLoc, mProjMatrix);

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
}

void GLWindow::closeEvent(QCloseEvent *event)
{
    mPreviewWidget->close();
}

void GLWindow::initShaderProgram()
{
    mRenderShader = new QOpenGLShaderProgram(this);
    mRenderShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ads.vert");
    mRenderShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ads.frag");
    mRenderShader->link();

    mRenderShader->bind();
    mPosAttr = mRenderShader->attributeLocation("posAttr");
    mColAttr = mRenderShader->attributeLocation("colAttr");
    mNormalAttr = mRenderShader->attributeLocation("normalAttr");
    mMatrixLoc = mRenderShader->uniformLocation("matrix");
    mProjLoc = mRenderShader->uniformLocation("projMatrix");
    mRenderShader->release();
}
