#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
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

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    void timerEvent(QTimerEvent *event);

private:
    void initShaderProgram();

private:
    QOpenGLVertexArrayObject mVAO;

    QMatrix4x4 mProjMatrix;
    QOpenGLShaderProgram *mShadowShader;
    QOpenGLShaderProgram *mRenderShader;
    Box *mBox;

    float mRotAngle;

    GLuint depTextureId;
    GLuint depFbo;

    GLuint mAVertex;
    GLuint mAColor;
    GLuint mANormal;
    GLuint mUModelMatrix;
    GLuint mUViewMatrix;
    GLuint mUProjMatrix;
    GLuint mUBiasMatrix;
    GLuint mUShadowMatrix;
    GLuint mUNormalMatrix;
    GLuint mULightDirection;
    GLuint mUShadowTexture;

    GLuint mADepPos;
    GLuint mUDepModelMatrix;
    GLuint mUDepViewMatrix;
    GLuint mUDepProjMatrix;
};

#endif // GLWINDOW_H
