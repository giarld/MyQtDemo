#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>

class PreviewWidget;
class Box;

class GLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWindow(QWidget *parent = 0);
    ~GLWindow();

    // QOpenGLWidget interface
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void initShaderProgram();

private:
    QOpenGLVertexArrayObject mVAO;

    QMatrix4x4 mProjMatrix;
    QOpenGLShaderProgram *mShadowShader;
    QOpenGLShaderProgram *mRenderShader;
    Box *mBox;

    GLuint mPosAttr;
    GLuint mColAttr;
    GLuint mNormalAttr;
    GLuint mMatrixLoc;
    GLuint mProjLoc;

    PreviewWidget *mPreviewWidget;
};

#endif // GLWINDOW_H
