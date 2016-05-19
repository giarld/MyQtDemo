#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QGLWidget>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QImage>


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QGLWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);

private :
    void loadGLTextures();

signals:

public slots:

private:
    GLuint texture[1];
    GLfloat rx,ry,rz;

};

#endif // GLWIDGET_H
