#include "glwidget.h"

GLWidget::GLWidget(QGLWidget *parent):
    QGLWidget(parent)
{
    rx=ry=rz=45.0;
}

void GLWidget::loadGLTexture()
{
    QImage buf,tex;
    if(!buf.load("./texture.png")){
        QImage temp(256,256,QImage::Format_RGB32);
        temp.fill(Qt::green);
        buf=temp;
    }
    tex=QGLWidget::convertToGLFormat(buf);
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,3,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());

    glColor4d(1.0,1.0,1.0,0.5);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void GLWidget::initializeGL()
{
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.5,1.0,0.5,0.5);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glTranslated(0.0,0.0,-5.0);
    glRotated(rx,1.0,0.0,0.0);
    glRotated(ry,0.0,1.0,0.0);
    glRotated(rz,0.0,0.0,1.0);

    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);
    // 前面
    glTexCoord2f(0.0,0.0); glVertex3f(-1.0,-1.0,1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(1.0,1.0); glVertex3f(1.0,1.0,1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(-1.0,1.0,1.0);

    // 后面
    glTexCoord2f(0.0,0.0); glVertex3f(1.0,-1.0,-1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(-1.0,-1.0,-1.0);
    glTexCoord2f(1.0,1.0); glVertex3f(-1.0,1.0,-1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(1.0,1.0,-1.0);

    // 顶面
    glTexCoord2f(0.0,0.0);glVertex3f(-1.0,1.0,1.0);
    glTexCoord2f(1.0,0.0);glVertex3f(1.0,1.0,1.0);
    glTexCoord2f(1.0,1.0);glVertex3f(1.0,1.0,-1.0);
    glTexCoord2f(0.0,1.0);glVertex3f(-1.0,1.0,-1.0);

    // 底面
    glTexCoord2f(0.0,0.0);glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(1.0,0.0);glVertex3f(-1.0,-1.0,1.0);
    glTexCoord2f(1.0,1.0);glVertex3f(-1.0,-1.0,-1.0);
    glTexCoord2f(0.0,1.0);glVertex3f(1.0,-1.0,-1.0);

    // 左面
    glTexCoord2f(0.0,0.0);glVertex3f(-1.0,-1.0,-1.0);
    glTexCoord2f(1.0,0.0);glVertex3f(-1.0,-1.0,1.0);
    glTexCoord2f(1.0,1.0);glVertex3f(-1.0,1.0,1.0);
    glTexCoord2f(0.0,1.0);glVertex3f(-1.0,1.0,-1.0);

    // 右面
    glTexCoord2f(0.0,0.0);glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(1.0,0.0);glVertex3f(1.0,-1.0,-1.0);
    glTexCoord2f(1.0,1.0);glVertex3f(1.0,1.0,-1.0);
    glTexCoord2f(0.0,1.0);glVertex3f(1.0,1.0,1.0);

    glEnd();

}

void GLWidget::resizeGL(int w, int h)
{
    if(h==0)
        h=1;

    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(GLdouble)w/(GLdouble)h,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::updateAmi()
{
    rx+=0.1;
    ry+=0.2;
    rz+=0.4;
    if(rx>360.0)
        rx=0.0;
    if(ry>360.0)
        ry=0.0;
    if(rz>360.0)
        rz=0.0;
}

void GLWidget::reBland(bool r)
{
    if(r){
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }
    else{
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}
