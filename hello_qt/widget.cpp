#include "widget.h"
#include <QLabel>
#include <QLayout>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mHelloLabel = new QLabel("Hello Qt in Widget", this);
    mButton = new QPushButton("Quit", this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mHelloLabel);
    layout->addWidget(mButton);
    this->setLayout(layout);

    connect(mButton, &QPushButton::clicked, this,  &Widget::close);
//    connect(mButton, SIGNAL(clicked(bool)), this, SLOT(close()));       //老的写法
}

Widget::~Widget()
{

}
