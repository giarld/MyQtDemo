#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget=new GLWidget();
    this->setCentralWidget(widget);
    showTimer=new QTimer();
    upTimer=new QTimer();
    connect(showTimer,SIGNAL(timeout()),widget,SLOT(updateGL()));
    showTimer->start(1000/60);
    connect(upTimer,SIGNAL(timeout()),widget,SLOT(updateAmi()));
    p=false;
    blend=false;
    this->setGeometry(300,100,640,480);
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_P){
        p=!p;
        if(p){
            upTimer->start(10);
        }
        else
            upTimer->stop();
    }
    else if(event->key()==Qt::Key_B){
        blend=!blend;
        widget->reBland(blend);
    }
}
