#include "glwidget.h"

//*********************************
double point[45][45][3];            // Points网格顶点数组
int wiggle_count=0;             // 指定旗形波浪的运动速度
GLdouble hold;
GLdouble xrot,yrot,zrot;

//************************************

GLWidget::GLWidget(QGLWidget *parent) :
    QGLWidget(parent)
{
    xrot=yrot=zrot=0.0;
    startTimer(1000/60);
}

GLWidget::~GLWidget()
{

}


void GLWidget::loadGLTexture()
{
    QImage tex,buf;
    if(!buf.load("./data/flag.png")){
        QImage temp(255,255,QImage::Format_RGB32);
        temp.fill(Qt::green);
        buf=temp;
    }
    tex=QGLWidget::convertToGLFormat(buf);
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,tex.width(),tex.height(),GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());

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

    glPolygonMode(GL_BACK,GL_FILL);      // 后表面完全填充
    glPolygonMode(GL_FRONT,GL_LINE);    // 前表面使用线条绘制

    for(int x=0;x<45;x++){
        for(int y=0;y<45;y++){
            point[x][y][0]=double(x/5.0-4.5);
            point[x][y][1]=double(y/5.0-4.5);
            point[x][y][2]=double(sin((((x/5.0)*40.0)/360)*3.141592654*2.0));
        }
    }
}

void GLWidget::paintGL()
{
    int x,y;
    double float_x,float_y,float_xb,float_yb;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslated(0.0,0.0,-15.0);
    glRotated(xrot,1.0,0.0,0.0);
    glRotated(yrot,0.0,1.0,0.0);
    glRotated(zrot,0.0,0.0,1.0);
    glBindTexture(GL_TEXTURE_2D,texture[0]);

    glBegin(GL_QUADS);
    for(x=0;x<44;x++){
        for(y=0;y<44;y++){
            float_x=(double)x/44.0;
            float_y=(double)y/44.0;
            float_xb=double(x+1)/44.0;
            float_yb=double(y+1)/44.0;

            glTexCoord2d(float_x,float_y);
            glVertex3d(point[x][y][0],point[x][y][1],point[x][y][2]);

            glTexCoord2d(float_x,float_yb);
            glVertex3d(point[x][y+1][0],point[x][y+1][1],point[x][y+1][2]);

            glTexCoord2d(float_xb,float_yb);
            glVertex3d(point[x+1][y+1][0],point[x+1][y+1][1],point[x+1][y+1][2]);

            glTexCoord2d(float_xb,float_y);
            glVertex3d(point[x+1][y][0],point[x+1][y][1],point[x+1][y][2]);

        }
    }
    glEnd();

    if(wiggle_count==2){
        for(y=0;y<45;y++){
            hold=point[0][y][2];
            for(x=0;x<44;x++){
                point[x][y][2]=point[x+1][y][2];
            }
            point[44][y][2]=hold;
        }
        wiggle_count=0;
    }
    wiggle_count++;

    if(xrot>359.0)
        xrot=0.0;
    else
        xrot+=0.3;

    if(yrot>359.0)
        yrot=0.0;
    else
        yrot+=0.2;

    if(zrot>359.0)
        zrot=0.0;
    else
        zrot+=0.4;
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

void GLWidget::timerEvent(QTimerEvent *)
{
    updateGL();
}
