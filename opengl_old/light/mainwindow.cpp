#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget=new GLWidget();
    this->setCentralWidget(widget);
    this->setGeometry(320,100,640,480);

    timer=new QTimer();
    connect(timer,SIGNAL(timeout()),widget,SLOT(updateGL()));
    p=0;
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_P){
        p=!p;
        if(p)
            timer->start(1000/60);
        else
            timer->stop();
    }
    else if(event->key()==Qt::Key_1)
        widget->setTexture(0);
    else if(event->key()==Qt::Key_2)
        widget->setTexture(1);
    else if(event->key()==Qt::Key_3)
        widget->setTexture(2);
    else if(event->key()==Qt::Key_W){
        widget->setZ(widget->getZ()-0.1);
    }
    else if(event->key()==Qt::Key_S){
        widget->setZ(widget->getZ()+0.1);
    }

}
