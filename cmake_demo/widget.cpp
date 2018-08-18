//
// Created by gxin on 18-8-18.
//

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
        : QWidget(parent),
          ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("CmakeDemo");
}

Widget::~Widget()
{
    delete ui;
}
