#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget=new GLWidget();
    this->setCentralWidget(widget);
    this->setGeometry(300,100,640,480);
    timer=new QTimer();
    connect(timer,SIGNAL(timeout()),widget,SLOT(updateGL()));
    timer->start(1000/60);

    inworld=false;
    mouseTimer=new QTimer();
    connect(mouseTimer,SIGNAL(timeout()),this,SLOT(mouseP()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::mouseP()
{
    QPoint op(this->pos().x()+this->width()/2,this->pos().y()+this->height()/2);
    if(inworld){
        int rx=this->cursor().pos().x()-op.x();
        double t=(double(rx)*1/16.0);
        widget->rolaX(t);
        this->cursor().setPos(op);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape){
        if(inworld){
            inworld=false;
            this->setCursor(Qt::ArrowCursor);
            mouseTimer->stop();
        }
    }
    else
        widget->key_press(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    widget->key_relese(event->key());
}

void MainWindow::mouseMoveEvent(QMouseEvent *)
{


}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(!inworld){
            inworld=true;
            mouseTimer->start(2);
           this->setCursor(Qt::BlankCursor);
        }
    }
}
