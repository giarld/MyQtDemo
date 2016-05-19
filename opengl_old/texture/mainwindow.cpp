#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget=new GLWidget();
    this->setCentralWidget(widget);
    this->setGeometry(300,200,640,480);

    timer=new QTimer();
    connect(timer,SIGNAL(timeout()),widget,SLOT(updateGL()));
    //timer->start(0);
    p=false;
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
}
