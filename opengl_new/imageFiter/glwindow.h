#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class GLWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWindow(QWidget *parent = 0);
    ~GLWindow();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    void initShaderProgram();

private:
    QMatrix4x4 mProjMatrix;
    QOpenGLShaderProgram *mShaderProgram;

    GLuint mPosAttr;
    GLuint mTexAttr;
    GLuint mMatrixLoc;
    GLuint mProjLoc;
};

#endif // WIDGET_H
