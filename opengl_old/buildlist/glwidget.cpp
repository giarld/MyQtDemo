#include "glwidget.h"

static GLdouble boxcol[5][3]={
    // 亮:红，橙，黄，绿，蓝
    {1.0,0.0,0.0},{1.0,0.5,0.0},{1.0,1.0,0.0},{0.0,1.0,0.0},{0.0,1.0,1.0}
};

static GLdouble topcol[5][3]={
    {0.5,0.0,0.0},{0.5,0.5,0.0},{0.5,0.5,0.0},{0.0,0.5,0.0},{0.0,0.5,0.5}
};

double xrot,yrot,zrot;
/////////////////////////////////////////////

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    xrot=yrot=zrot=0.0;
}

void GLWidget::BuildLists()
{
    box=glGenLists(2);
    glNewList(box,GL_COMPILE);

    glBegin(GL_QUADS);
    // 底面
    glTexCoord2d(1.0,1.0); glVertex3d(-1.0,-1.0,-1.0);
    glTexCoord2d(0.0,1.0); glVertex3d(1.0,-1.0,-1.0);
    glTexCoord2d(0.0,0.0); glVertex3d(1.0,-1.0,1.0);
    glTexCoord2d(1.0,0.0); glVertex3d(-1.0,-1.0,1.0);

    // 前面
    glTexCoord2d(0.0, 0.0); glVertex3d(-1.0, -1.0,  1.0);
    glTexCoord2d(1.0, 0.0); glVertex3d( 1.0, -1.0,  1.0);
    glTexCoord2d(1.0, 1.0); glVertex3d( 1.0,  1.0,  1.0);
    glTexCoord2d(0.0, 1.0); glVertex3d(-1.0,  1.0,  1.0);

    // 后面
    glTexCoord2d(1.0, 0.0); glVertex3d(-1.0, -1.0, -1.0);
    glTexCoord2d(1.0, 1.0); glVertex3d(-1.0,  1.0, -1.0);
    glTexCoord2d(0.0, 1.0); glVertex3d( 1.0,  1.0f, -1.0);
    glTexCoord2d(0.0, 0.0); glVertex3d(1.0, -1.0, -1.0);

    // 右面
    glTexCoord2d(1.0f, 0.0f); glVertex3d( 1.0f, -1.0f, -1.0f);
    glTexCoord2d(1.0f, 1.0f); glVertex3d( 1.0f,  1.0f, -1.0f);
    glTexCoord2d(0.0f, 1.0f); glVertex3d( 1.0f,  1.0f,  1.0f);
    glTexCoord2d(0.0f, 0.0f); glVertex3d( 1.0f, -1.0f,  1.0f);

    // 左面
    glTexCoord2d(0.0f, 0.0f); glVertex3d(-1.0f, -1.0f, -1.0f);
    glTexCoord2d(1.0f, 0.0f); glVertex3d(-1.0f, -1.0f,  1.0f);
    glTexCoord2d(1.0f, 1.0f); glVertex3d(-1.0f,  1.0f,  1.0f);
    glTexCoord2d(0.0f, 1.0f); glVertex3d(-1.0f,  1.0f, -1.0f);

    glEnd();
    glEndList();

    top=box+1;
    glNewList(top,GL_COMPILE);
    glBegin(GL_QUADS);

    glTexCoord2d(0.0,1.0); glVertex3d(-1.0,1.0,-1.0);
    glTexCoord2d(0.0,0.0); glVertex3d(-1.0,1.0,1.0);
    glTexCoord2d(1.0,0.0); glVertex3d(1.0,1.0,1.0);
    glTexCoord2d(1.0,1.0); glVertex3d(1.0,1.0,-1.0);

    glEnd();
    glEndList();

}

void GLWidget::loadGLTexture()
{
    QImage tex,buf;
    if(!buf.load("./data/texture.png")){
        QImage temp(128,128,QImage::Format_RGB32);
        temp.fill(Qt::green);
        buf=temp;
    }
    tex=QGLWidget::convertToGLFormat(buf);
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,tex.width(),tex.height(),GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());

    BuildLists();

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

}

void GLWidget::initializeGL()
{
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);

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

    glBindTexture(GL_TEXTURE_2D,texture[0]);

    for(yloop=1;yloop<6;yloop++){
        for(xloop=0;xloop<yloop;xloop++){
            glLoadIdentity();
            glTranslated(1.4+(double(xloop)*2.8)-(double(yloop)*1.4),((6.0-double(yloop))*2.4)-7.0,-20.0);

            glRotated(45.0-(2.0*yloop)+xrot,1.0,0.0,0.0);
            glRotated(45.0+yrot,0.0,1.0,0.0);
            glColor3dv(boxcol[yloop-1]);
            glCallList(box);

            glColor3dv(topcol[yloop-1]);
            glCallList(top);
        }
    }
}
