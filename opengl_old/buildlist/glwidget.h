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
    GLWidget(QWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    void BuildLists();
    void loadGLTexture();

private:
    GLuint box,top;
    GLuint xloop;
    GLuint yloop;

    GLuint texture[1];

};

#endif // GLWIDGET_H
