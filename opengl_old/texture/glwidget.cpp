#include "glwidget.h"

GLWidget::GLWidget(QGLWidget *parent) :
    QGLWidget(parent)
{
    rx=ry=rz=45.0;
}

GLWidget::~GLWidget()
{

}

void GLWidget::loadGLTextures()
{
    QImage tex,buf;
    if(!buf.load("./texture.png")){
        QImage temp(128,128,QImage::Format_RGB32);
        temp.fill(Qt::green);
        buf=temp;
    }
    tex=QGLWidget::convertToGLFormat(buf);
    glGenTextures(1,&texture[0]);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glTexImage2D(GL_TEXTURE_2D,0,3,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE
                 ,tex.bits());

    //精细的纹理过滤
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);             //缩小的
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);             //放大的

    //粗糙的纹理过滤 (minecraft使用的就是这种方式 )
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
}

void GLWidget::initializeGL()
{
    loadGLTextures();
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0,0.0,0.0,0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0,0.0,-5.0);
    glRotatef(rx,1.0,0.0,0.0);
    glRotatef(ry,0.0,1.0,0.0);
    glRotatef(rz,0.0,0.0,1.0);

    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);
    // 前面
    glTexCoord2f(0.0,0.0); glVertex3f(-1.0,-1.0,1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(1.0,1.0); glVertex3f(1.0,1.0,1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(-1.0,1.0,1.0);

    // 后面
    glTexCoord2f(0.0,0.0); glVertex3f(-1.0,-1.0,-1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(1.0,-1.0,-1.0);
    glTexCoord2f(1.0,1.0); glVertex3f(1.0,1.0,-1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(-1.0,1.0,-1.0);

    // 顶面
    glTexCoord2f(0.0,0.0);glVertex3f(-1.0,1.0,1.0);
    glTexCoord2f(1.0,0.0);glVertex3f(1.0,1.0,1.0);
    glTexCoord2f(1.0,1.0);glVertex3f(1.0,1.0,-1.0);
    glTexCoord2f(0.0,1.0);glVertex3f(-1.0,1.0,-1.0);

    // 底面
    glTexCoord2f(0.0,0.0);glVertex3f(-1.0,-1.0,1.0);
    glTexCoord2f(1.0,0.0);glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(1.0,1.0);glVertex3f(1.0,-1.0,-1.0);
    glTexCoord2f(0.0,1.0);glVertex3f(-1.0,-1.0,-1.0);

    // 左面
    glTexCoord2f(0.0,0.0);glVertex3f(-1.0,-1.0,-1.0);
    glTexCoord2f(1.0,0.0);glVertex3f(-1.0,-1.0,1.0);
    glTexCoord2f(1.0,1.0);glVertex3f(-1.0,1.0,1.0);
    glTexCoord2f(0.0,1.0);glVertex3f(-1.0,1.0,-1.0);

    // 右面
    glTexCoord2f(0.0,0.0);glVertex3f(1.0,-1.0,-1.0);
    glTexCoord2f(1.0,0.0);glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(1.0,1.0);glVertex3f(1.0,1.0,1.0);
    glTexCoord2f(0.0,1.0);glVertex3f(1.0,1.0,-1.0);

    glEnd();

    rx+=0.5;
    ry+=0.8;
    rz+=1.0;
    if(rx>=360.0)
        rx=0.0;
    if(ry>=360.0)
        ry=0.0;
    if(rz>=360.0)
        rz=0.0;
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

