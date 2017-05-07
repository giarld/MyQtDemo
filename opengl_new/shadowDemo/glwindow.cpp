#include "glwindow.h"
#include <QOpenGLFramebufferObject>

GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setVersion(3,3);
    this->setFormat(fmt);
}

GLWindow::~GLWindow()
{

}

void GLWindow::initializeGL()
{
    QOpenGLFunctions *gl = this->context()->functions();
    gl->glClearColor(0, 0, 0, 1.0f);

    QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setComparisonMode(QOpenGLTexture::CompareRefToTexture);
    texture->setComparisonFunction(QOpenGLTexture::CompareLessEqual);
}

void GLWindow::resizeGL(int w, int h)
{
    mProjMatrix.setToIdentity();
    mProjMatrix.perspective(70.0f, GLfloat(w) / h, 0.001f, 10000.0f);
}

void GLWindow::paintGL()
{

}
