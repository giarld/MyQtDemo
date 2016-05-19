#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QImage>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QGLWidget *parent = 0);
    ~GLWidget();
    double getZ();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);

private:
    void loadGLTexture();

signals:

public slots:
    void setTexture(int a);
    void setZ(double i);

private:
    GLdouble rx,ry,rz;
    GLdouble z;
    int filter;                 // 滤波类型
    GLuint texture[3];
    int textureIndex;
};

#endif // GLWIDGET_H
