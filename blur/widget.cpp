#include "widget.h"
#include <QImage>
#include <QPainter>
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    ,mBlurLever(0)
{
    mMThread = new QThread(this);
    mMake = new MakeBlurImage;
    mMake->moveToThread(mMThread);
    connect(mMThread, &QThread::destroyed, mMake, &QObject::deleteLater);

    connect(this, &Widget::toMake, mMake, &MakeBlurImage::makeBlur);
    connect(mMake, &MakeBlurImage::makeOver, this, &Widget::Over);      //Widget线程为主线程 不需要Qt::DirectConnection标志。

    mMThread->start();
    mMake->setImage(":/test.png");
}

Widget::~Widget()
{
    mMThread->quit();
    mMThread->wait();
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(QRectF(0,0,this->width(),this->height()),mImage);
}

void Widget::keyPressEvent(QKeyEvent *key)
{
    if(key->key() == Qt::Key_Up){
        mBlurLever++;
        emit toMake(mBlurLever);
    }
    else if(key->key() == Qt::Key_Down){
        mBlurLever--;
        if(mBlurLever<0) mBlurLever = 0;
        emit toMake(mBlurLever);
    }
}

void Widget::Over(const QImage &img)
{
    mImage = img;
    update();
}
