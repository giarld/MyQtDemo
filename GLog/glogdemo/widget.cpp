#include "widget.h"
#include "ui_widget.h"
#include <glog.h>
#include <QDateTime>
#include <QPushButton>
#include <QLayout>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    gLogDebug("Widget created");

    QPushButton *button = new QPushButton("put time", this);
    QLayout *layout = new QHBoxLayout;
    layout->addWidget(button);
    this->setLayout(layout);

    connect(button, &QPushButton::clicked, [this] () {
        gLogInfo(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    });
}

Widget::~Widget()
{
    delete ui;
}
