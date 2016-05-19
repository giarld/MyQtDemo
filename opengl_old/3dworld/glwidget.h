#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QPainter>

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
    void timerEvent(QTimerEvent *);

public slots:
    void keyBuff();
    void key_relese(int key);
    void key_press(int key);
    void rolaX(double rx);

private:
    void loadGLTexture();
    void setUpWorld();
    void loadMap(const QStringList &map);

private:
    GLuint texture[0];
};

#endif // GLWIDGET_H
