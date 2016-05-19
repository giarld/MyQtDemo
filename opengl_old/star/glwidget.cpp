#include "glwidget.h"

GLWidget::GLWidget(QGLWidget *parent) :
    QGLWidget(parent)
{
    this->zoom=-15.0;
    this->tilt=90.0;
    twinkle=1;
}

GLWidget::~GLWidget()
{

}

void GLWidget::loadGLTexture()
{
    QImage buf,tex;
    if(!buf.load("./Star.png")){
        QImage temp(128,128,QImage::Format_RGB32);
        temp.fill(Qt::green);
        buf=temp;
    }
    tex=QGLWidget::convertToGLFormat(buf);
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,3,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());
}

void GLWidget::initializeGL()
{
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);    // 启用阴影平滑
    glClearColor(0.0,0.0,0.0,0.0);
    glClearDepth(1.0);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    //这里我们使用了纹理映射
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glEnable(GL_BLEND);
    qsrand(time(0));
    for(int loop=0;loop<NUM;loop++){
        star[loop].angle=0.0;
        star[loop].dist=((double)loop/NUM)*5.0;
        star[loop].r=qrand()%256;
        star[loop].g=qrand()%256;
        star[loop].b=qrand()%256;
    }
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    //glLoadIdentity();

    for(int i=0;i<NUM;i++){
        glLoadIdentity();
       glTranslated(0.0,0.0,zoom);
       glRotated(tilt,1.0,0.0,0.0);
       glRotated(star[i].angle,0.0,1.0,0.0);
       glTranslated(star[i].dist,0.0,0.0);
       glRotated(-star[i].angle,0.0,1.0,0.0);
       glRotated(-tilt,1.0,0.0,0.0);

       if(twinkle){
           glColor4ub(star[NUM-i-1].r,star[NUM-i-1].g,star[NUM-i-1].b,255);
            glBegin(GL_QUADS);
                glTexCoord2d(0.0,0.0); glVertex3d(-1.0,-1.0,0.0);
                glTexCoord2d(1.0,0.0); glVertex3d(1.0,-1.0,0.0);
                glTexCoord2d(1.0,1.0); glVertex3d(1.0,1.0,0.0);
                glTexCoord2d(0.0,1.0); glVertex3d(-1.0,1.0,0.0);
            glEnd();
       }
       glRotated(spin,0.0,0.0,1.0);
       glColor4ub(star[i].r,star[i].g,star[i].b,255);

       glBegin(GL_QUADS);
           glTexCoord2d(0.0,0.0); glVertex3d(-1.0,-1.0,0.0);
           glTexCoord2d(1.0,0.0); glVertex3d(1.0,-1.0,0.0);
           glTexCoord2d(1.0,1.0); glVertex3d(1.0,1.0,0.0);
           glTexCoord2d(0.0,1.0); glVertex3d(-1.0,1.0,0.0);
        glEnd();

        spin+=0.01;
        star[i].angle+=((double)i/NUM);
        star[i].dist-=0.01;

        if(star[i].dist<0.0){
            star[i].dist+=5.0;
            star[i].r=qrand()%256;
            star[i].g=qrand()%256;
            star[i].b=qrand()%256;
        }
    }
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
