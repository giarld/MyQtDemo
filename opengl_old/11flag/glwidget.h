#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <QTimerEvent>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QGLWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void timerEvent(QTimerEvent *);

private:
    void loadGLTexture();

private:
    GLuint texture[1];
};

#endif // GLWIDGET_H
