#include "glwidget.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <math.h>


// 创建Vertex顶点结构
struct VERTEX{
    double x,y,z;
    double u,v;
};

// 创建Triangle三角形结构
struct TRIANGLE{
    VERTEX vertex[3];
};

// 创建Sector区段结构
struct SECTOR{
    int numtriangles;
    TRIANGLE *triangle;
};

SECTOR sector1;     //创建一个Sector区段

const double piover180=0.0174532925;        //一个很简单的折算因子用来折算度和弧度

double walkbias;                    //视角摆动的幅度
double walkbiasangle;
double yrot;
double lookupdown;
double xpos,zpos;
double z;
double heading;

bool key_r,key_l,key_u,key_d;
////////////////////////////////////////////////

GLWidget::GLWidget(QGLWidget *parent) :
    QGLWidget(parent)
{
    walkbias=0.0;
    walkbiasangle=0.0;
    lookupdown=0.0;
    z=0.0;
    yrot=0.0;
    xpos=zpos=0.0;
    setUpWorld();
    startTimer(1000/60);
    key_d=key_r=key_l=key_u=false;
}

GLWidget::~GLWidget()
{
}


void GLWidget::loadGLTexture()
{
    QImage buf,tex;
    if(!buf.load("./data/texture.png")){
        QImage temp(32,32,QImage::Format_RGB32);
        temp.fill(Qt::green);
        buf=temp;
    }
    tex=QGLWidget::convertToGLFormat(buf);
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,tex.width(),tex.height(),GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());
}

void GLWidget::loadMap(const QStringList &map)
{
    //qDebug()<<map.size();
    double x,y,z,u,v;
    int rt=0;int loops=0;
    sector1.numtriangles=map.size();
    sector1.triangle=new TRIANGLE[sector1.numtriangles];
    for(int i=0;i<map.size();i++){
        QStringList temp=map[i].split(" ");
        temp.removeAll("");
        x=temp[0].toDouble();
        y=temp[1].toDouble();
        z=temp[2].toDouble();
        u=temp[3].toDouble();
        v=temp[4].toDouble();
        sector1.triangle[loops].vertex[rt].x=x;
        sector1.triangle[loops].vertex[rt].y=y;
        sector1.triangle[loops].vertex[rt].z=z;
        sector1.triangle[loops].vertex[rt].u=u;
        sector1.triangle[loops].vertex[rt].v=v;
        rt++;
        loops+=rt/3;
        rt%=3;
    }
}

void GLWidget::key_relese(int key)
{
    if(key==Qt::Key_Right || key==Qt::Key_D){
        key_r=0;
    }
    else if(key==Qt::Key_Left || key==Qt::Key_A)
        key_l=0;
    else if(key==Qt::Key_Up || key==Qt::Key_W)
        key_u=0;
    else if(key==Qt::Key_Down  || key==Qt::Key_S)
        key_d=0;
}

void GLWidget::key_press(int key)
{

    if(key==Qt::Key_Right || key==Qt::Key_D){
        key_r=1;
    }
    else if(key==Qt::Key_Left || key==Qt::Key_A)
        key_l=1;
    else if(key==Qt::Key_Up || key==Qt::Key_W)
        key_u=1;
    else if(key==Qt::Key_Down || key==Qt::Key_S)
        key_d=1;
}

void GLWidget::rolaX(double rx)
{
    heading-=rx;
    yrot=heading;
}

void GLWidget::setUpWorld()
{
    QFile file("./data/World.txt");
    QStringList map;
    map.clear();
    if(file.open(QFile::ReadOnly)){
        QTextStream in(&file);
        while(!in.atEnd()){
            QString temp;
            temp=in.readLine();
            if(!temp.isEmpty() && temp[0]!='/')
                map.append(temp);
        }
    }
    loadMap(map);

    //qDebug()<<map;
    map.clear();
}

void GLWidget::initializeGL()
{
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0,0.5,1.0,0.5);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

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
    keyBuff();
}

void GLWidget::keyBuff()
{
    if(key_r){
        heading-=1.5;
        yrot=heading;
    }

    if(key_l){
        heading+=1.5;
        yrot=heading;
    }

    if(key_u){
        xpos-=((double)sin(heading*piover180)*0.05);        // 沿游戏者所在的X平面移动
        zpos-=((double)cos(heading*piover180)*0.05);    // 沿游戏者所在的Z平面移动
        if(walkbiasangle>=359.0)
            walkbiasangle=0.0;
        else
            walkbiasangle+=10;
        walkbias=(double)sin(walkbiasangle*piover180)/20.0;     // 使游戏者产生跳跃感
    }
    if(key_d){
        xpos+=(sin(heading*piover180)*0.05);
        zpos+=(cos(heading*piover180)*0.05);
        if(walkbiasangle<=1.0)
            walkbiasangle=359.0;
        else
            walkbiasangle-=10;
        walkbias=(double)sin(walkbiasangle*piover180)/20.0;
    }
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GLdouble x_m,y_m,z_m,u_m,v_m;
    GLdouble xtrans=-xpos;
    GLdouble ztrans=-zpos;
    GLdouble ytrans=-walkbias-0.25;
    GLdouble sceneroty=360.0-yrot;
    int numtriangles;

    glRotated(lookupdown,1.0,0.0,1.0);
    glRotated(sceneroty,0.0,1.0,0.0);

    glTranslated(xtrans,ytrans,ztrans);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    numtriangles=sector1.numtriangles;

    for(int loop_m=0;loop_m<numtriangles;loop_m++){
        glBegin(GL_TRIANGLES);
            glNormal3d(0.0,0.0,1.0);
            x_m=sector1.triangle[loop_m].vertex[0].x;
            y_m=sector1.triangle[loop_m].vertex[0].y;
            z_m=sector1.triangle[loop_m].vertex[0].z;
            u_m=sector1.triangle[loop_m].vertex[0].u;
            v_m=sector1.triangle[loop_m].vertex[0].v;
            glTexCoord2d(u_m,v_m);glVertex3d(x_m,y_m,z_m);

            x_m=sector1.triangle[loop_m].vertex[1].x;
            y_m=sector1.triangle[loop_m].vertex[1].y;
            z_m=sector1.triangle[loop_m].vertex[1].z;
            u_m=sector1.triangle[loop_m].vertex[1].u;
            v_m=sector1.triangle[loop_m].vertex[1].v;
            glTexCoord2d(u_m,v_m);glVertex3d(x_m,y_m,z_m);

            x_m=sector1.triangle[loop_m].vertex[2].x;
            y_m=sector1.triangle[loop_m].vertex[2].y;
            z_m=sector1.triangle[loop_m].vertex[2].z;
            u_m=sector1.triangle[loop_m].vertex[2].u;
            v_m=sector1.triangle[loop_m].vertex[2].v;
            glTexCoord2d(u_m,v_m);glVertex3d(x_m,y_m,z_m);

        glEnd();

    }

    glLoadIdentity();
    glTranslated(0.0,0.0,-0.1);

    glBegin(GL_LINES);
        glColor3d(1.0,1.0,1.0);
        glVertex2d(-0.002,0.0);
        glVertex2d(0.002,0.0);

        glVertex2d(0.0,0.002);
        glVertex2d(0.0,-0.002);
    glEnd();

//    glDisable(GL_DEPTH_TEST);
//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//    glLoadIdentity();
//    //glOrtho(0,640,0,480,-1,1);
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glLoadIdentity();

//    glRectd(-0.5,0.0,0.5,0.25);

//    glMatrixMode(GL_PROJECTION);
//    glPopMatrix();
//    glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();
//    glEnable(GL_DEPTH_TEST);
}
