#include "mainwindow.h"

//三种不同的雾的计算方法，以及怎样设置雾的颜色和雾的范围。


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget=new glwidget();
    this->setCentralWidget(widget);
    this->setGeometry(300,100,640,480);
}

MainWindow::~MainWindow()
{

}
