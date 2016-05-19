#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget=new GLWidget();
    this->setCentralWidget(widget);
    this->setGeometry(300,100,640,480);
}

MainWindow::~MainWindow()
{

}
