#include "glwidget.h"
#include <string.h>

GLWidget::GLWidget(QGLWidget *parent) :
    QGLWidget(parent)
{
    cnt1=cnt2=0.0;
    startTimer(1000/60);
}

GLWidget::~GLWidget()
{
    glDeleteLists(base,256);
}

void GLWidget::initializeGL()
{
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);

    buildFont();

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0,0.0,0.0,0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
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

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D,texture[1]);
    glTranslated(0,0,-5.0);
    glRotated(45.0,0,0,1);
    glRotated(cnt1*30.0,1,1,1);
    glDisable(GL_BLEND);
    glColor3d(1.0,1.0,1.0);
    glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2d(-1.0,1.0);
        glTexCoord2d(1.0,0.0);
        glVertex2d(1.0,1.0);
        glTexCoord2d(1.0,1.0);
        glVertex2d(1.0,-1.0);
        glTexCoord2d(0.0,1.0);
        glVertex2d(-1.0,-1.0);
    glEnd();

    glRotated(90.0,1.0,1.0,0.0);
    glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2d(-1.0,1.0);
        glTexCoord2d(1.0,0.0);
        glVertex2d(1.0,1.0);
        glTexCoord2d(1.0,1.0);
        glVertex2d(1.0,-1.0);
        glTexCoord2d(0.0,1.0);
        glVertex2d(-1.0,-1.0);
    glEnd();

    glEnable(GL_BLEND);
    glLoadIdentity();
    glColor3d(1.0*(cos(cnt1)),1.0*sin(cnt2),1.0-0.5*cos(cnt1+cnt2));

    glPrint(int(280+250*cos(cnt1)),int(235+200*sin(cnt2)),"GxIn",0);

    glColor3d(1.0*sin(cnt2),1.0-0.5*cos(cnt1+cnt2),1.0*cos(cnt1));
    glPrint(int(280+230*cos(cnt2)),int(235+200*sin(cnt1)),"OpenGL",1);

    glColor3d(0.0,0.0,1.0);
    glPrint(int(240+200*cos((cnt2+cnt1)/5)),2,"Giuseppe D'Agata",0);

    if(cnt1>=360.0)
        cnt1=0.0;
    else
        cnt1+=0.01;

    if(cnt2>=360.0)
        cnt2=0.0;
    else
        cnt2+=0.0081;
}

void GLWidget::timerEvent(QTimerEvent *)
{
    updateGL();
}

void GLWidget::loadGLTexture()
{
    QImage tex[2],buf;
    QImage fu(256,256,QImage::Format_RGB32);
    fu.fill(Qt::green);
    if(!buf.load("./data/Font.png"))
    {
        buf=fu;
    }
    tex[0]=QGLWidget::convertToGLFormat(buf);
    buf.load("./data/Bumps.png");
    tex[1]=QGLWidget::convertToGLFormat(buf);
    glGenTextures(2,texture);
    for(int i=0;i<2;i++){
        glBindTexture(GL_TEXTURE_2D,texture[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,3,tex[i].width(),tex[i].height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex[i].bits());
    }
}

void GLWidget::buildFont()
{
    double cx,cy;
    base=glGenLists(256);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    for(int i=0;i<256;i++){
        cx=double(i%16)/16.0;
        cy=double(i/16)/16.0;
        glNewList(base+i,GL_COMPILE);
        glBegin(GL_QUADS);
            glTexCoord2d(cx,1-cy-0.0625); glVertex2i(0,0);
            glTexCoord2d(cx+0.0625,1-cy-0.0625); glVertex2i(16,0);
            glTexCoord2d(cx+0.0625,1-cy); glVertex2i(16,16);
            glTexCoord2d(cx,1-cy); glVertex2i(0,16);
        glEnd();
        glTranslated(16,0,0);
        glEndList();
    }
}

void GLWidget::glPrint(int x, int y, char *string, int set)
{
    if(set>1)
        set=1;
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,640,0,480,-1,1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(x,y,0);
    glListBase(base-32+(128*set));
    glCallLists(strlen(string),GL_UNSIGNED_BYTE,string);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}
