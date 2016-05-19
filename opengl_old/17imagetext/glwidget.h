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
    explicit GLWidget(QGLWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void timerEvent(QTimerEvent *);

private:
    void loadGLTexture();
    void buildFont();
    void glPrint(int x,int y,char *string,int set);

    GLuint base;
    GLuint texture[2];
    GLdouble cnt1,cnt2;
};

#endif // GLWIDGET_H
