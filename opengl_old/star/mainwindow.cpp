#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget=new GLWidget();
    this->setCentralWidget(widget);

    this->setGeometry(300,100,640,480);

    timer=new QTimer();
    connect(timer,SIGNAL(timeout()),widget,SLOT(updateGL()));
    timer->start(1000/60);\

    full=false;
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_F2){
        full=!full;
        if(full)
            this->showFullScreen();
        else
            this->showNormal();
    }
}
