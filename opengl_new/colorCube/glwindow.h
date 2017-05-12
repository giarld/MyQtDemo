#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector>
#include <QOpenGLVertexArrayObject>

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
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    void initShaderProgram();

private:
    QOpenGLVertexArrayObject mVAO;
    QOpenGLShaderProgram *mShaderProgram;
    QMatrix4x4 mProjMatrix;

    GLuint mPosAttr;
    GLuint mColAttr;
    GLuint mMatrixLoc;
    GLuint mProjLoc;

    QOpenGLBuffer mBuffer;
    GLuint mVertexCount;
};

struct Vertex {
    QVector3D positon;
    QVector3D color;
};

#endif // GLWINDOW_H
