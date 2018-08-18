#include "widget.h"
#include "ui_widget.h"
#include "httpserver.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    mHttpServer = new HttpServer(this);
}

Widget::~Widget()
{
    delete ui;
}
