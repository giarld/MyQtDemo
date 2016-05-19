#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/glut.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QGLWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);

public slots:
    void updateAmi();
    void reBland(bool r);

private:
    void loadGLTexture();

    GLuint texture[1];
    GLdouble rx,ry,rz;
};

#endif // GLWIDGET_H
