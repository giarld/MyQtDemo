#include "glwidget.h"

GLint fogMode[]={GL_EXP,GL_EXP2,GL_LINEAR};
GLfloat fogColor[4]={0.5,0.5,0.5,1.0};
GLfloat LightAmbient[]={0.5,0.5,0.5,1.0};      //环境光
GLfloat LightDiffuse[]={1.0,1.0,1.0,1.0};      //漫射光
GLfloat lightPosition[]={0.0,0.0,2.0,1.0};     // 光源位置
///-----------------------------------

glwidget::glwidget(QGLWidget *parent) :
    QGLWidget(parent)
{
    filter=2;foggilter=2;
    rx=ry=rz=0.0;
    z=-5.0;
    startTimer(1000/60);
}

void glwidget::initializeGL()
{
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.5,0.5,0.5,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glLightfv(GL_LIGHT1,GL_AMBIENT,LightAmbient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,LightDiffuse);
    glLightfv(GL_LIGHT1,GL_POSITION,lightPosition);
    glEnable(GL_LIGHT1);

    glFogi(GL_FOG_MODE,fogMode[foggilter]);     // 设置雾气的模式
    glFogfv(GL_FOG_COLOR,fogColor);                 // 设置雾的颜色
    glFogf(GL_FOG_DENSITY,0.35);                     // 设置雾的密度
    glHint(GL_FOG_HINT,GL_DONT_CARE);   // 设置系统如何计算雾气
    glFogf(GL_FOG_START,3.0);                               // 雾气的开始位置
    glFogf(GL_FOG_END,5.0);                             // 雾气的结束位置
    glEnable(GL_FOG);
}

void glwidget::loadGLTexture()
{
    QImage  buf,tex;
    if(!buf.load("./texture.png")){
        QImage temp(256,256,QImage::Format_RGB32);
        temp.fill(Qt::green);
        buf=temp;
    }
    tex=QGLWidget::convertToGLFormat(buf);
    glGenTextures(3,texture);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,3,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());

    glBindTexture(GL_TEXTURE_2D,texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,3,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());

    glBindTexture(GL_TEXTURE_2D,texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,tex.width(),tex.height(),GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());
}

void glwidget::resizeGL(int w, int h)
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

void glwidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_LIGHTING);

    glTranslatef(0.0,0.0,z);
    glRotated(rx,1.0,0.0,0.0);
    glRotated(ry,0.0,1.0,0.0);
    glRotated(rz,0.0,0.0,1.0);

    glBindTexture(GL_TEXTURE_2D,texture[filter]);
    glBegin(GL_QUADS);
    // 前侧面
    glNormal3d(0.0,0.0,1.0);       // 法线y
    glTexCoord2d(0.0,0.0);  glVertex3d(-1.0,-1.0,1.0);
    glTexCoord2d(1/4.0,0.0); glVertex3d(1.0,-1.0,1.0);
    glTexCoord2d(1/4.0,1/4.0); glVertex3d(1.0,1.0,1.0);
    glTexCoord2d(0.0,1/4.0); glVertex3d(-1.0,1.0,1.0);

    // 后侧面
    glNormal3d(0.0,0.0,-1.0);
    glTexCoord2d(0.0,0.0); glVertex3d(1.0,-1.0,-1.0);
    glTexCoord2d(1/4.0,0.0); glVertex3d(-1.0,-1.0,-1.0);
    glTexCoord2d(1/4.0,1/4.0); glVertex3d(-1.0,1.0,-1.0);
    glTexCoord2d(0.0,1/4.0); glVertex3d(1.0,1.0,-1.0);

    // 顶面
    glNormal3d(0.0,1.0,0.0);
    glTexCoord2d(1/4.0,0.0); glVertex3d(-1.0,1.0,1.0);
    glTexCoord2d(1/2.0,0.0); glVertex3d(1.0,1.0,1.0);
    glTexCoord2d(1/2.0,1/4.0); glVertex3d(1.0,1.0,-1.0);
    glTexCoord2d(1/4.0,1/4.0); glVertex3d(-1.0,1.0,-1.0);

    // 底面
    glNormal3d(0.0,-1.0,0.0);
    glTexCoord2d(0.0,1/4.0); glVertex3d(-1.0,-1.0,1.0);
    glTexCoord2d(1/4.0,1/4.0); glVertex3d(1.0,-1.0,1.0);
    glTexCoord2d(1/4.0,1/2.0); glVertex3d(1.0,-1.0,-1.0);
    glTexCoord2d(0.0,1/2.0); glVertex3d(-1.0,-1.0,-1.0);

    // 左侧面
    glNormal3d(-1.0,0.0,0.0);
    glTexCoord2d(0.0,0.0); glVertex3d(-1.0,-1.0,-1.0);
    glTexCoord2d(1/4.0,0.0); glVertex3d(-1.0,-1.0,1.0);
    glTexCoord2d(1/4.0,1/4.0); glVertex3d(-1.0,1.0,1.0);
    glTexCoord2d(0.0,1/4.0); glVertex3d(-1.0,1.0,-1.0);

    // 右侧面
    glNormal3d(1.0,0.0,0.0);
    glTexCoord2d(0.0,0.0); glVertex3d(1.0,-1.0,1.0);
    glTexCoord2d(1/4.0,0.0); glVertex3d(1.0,-1.0,-1.0);
    glTexCoord2d(1/4.0,1/4.0); glVertex3d(1.0,1.0,-1.0);
    glTexCoord2d(0.0,1/4.0); glVertex3d(1.0,1.0,1.0);

    glEnd();


    if(rx>359.0)
        rx=0.0;
    else
        rx+=0.1;

    if(ry>359.0)
        ry=0.0;
    else
        ry+=0.2;

    if(rz>359.0)
        rz=0.0;
    else
        rz+=0.4;
}

void glwidget::timerEvent(QTimerEvent *)
{
    updateGL();
}


