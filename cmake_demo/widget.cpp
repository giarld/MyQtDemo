//
// Created by gxin on 18-8-18.
//

#include "widget.h"
#include "ui_widget.h"
#include <QImage>
#include <QPainter>

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

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QImage image(":/pic.png");
    painter.drawImage(QRect(0, 0, this->width(), this->height()), image);
}
