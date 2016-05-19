#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <QTimerEvent>

class glwidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit glwidget(QGLWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void timerEvent(QTimerEvent *);

private:
    void loadGLTexture();
    int filter;
    int foggilter;

    double rx,ry,rz,z;

    GLuint texture[3];
};

#endif // GLWIDGET_H
