#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QImage>
#include <time.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QGLWidget *parent = 0);
    ~GLWidget();

private:
    void loadGLTexture();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

signals:

private:
    enum{NUM=50};
    GLuint texture[1];

    struct Stars{
        int r,g,b;
        GLdouble dist;
        GLdouble angle;
    };
    Stars star[NUM];

    GLdouble zoom;      // 星星离观察者的距离
    GLdouble tilt;           // 星星的倾角
    GLdouble spin;          // 闪烁星星的自转

    bool twinkle;
};

#endif // GLWIDGET_H
