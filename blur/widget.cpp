#include "widget.h"
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <QLayout>
#include <QProgressBar>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    ,mBlurLever(0)
{
    mSlider = new QSlider(Qt::Orientation::Vertical, this);
    mProgress = new QProgressBar(this);
    QHBoxLayout *alayout = new QHBoxLayout;
    alayout->addStretch(1);
    alayout->addWidget(mSlider);
    QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->addLayout(alayout);
    mLayout->addWidget(mProgress);
    this->setLayout(mLayout);
    this->setGeometry(100,100, 640,480);
    mSlider->setRange(0, 12);

    mMThread = new QThread(this);
    mMake = new MakeBlurImage;
    mMake->moveToThread(mMThread);
    connect(mMThread, &QThread::destroyed, mMake, &QObject::deleteLater);

    connect(mSlider, &QSlider::valueChanged, this, &Widget::toMake);
    connect(this, &Widget::mitMake, mMake, &MakeBlurImage::makeBlur);
    connect(mMake, &MakeBlurImage::makeOver, this, &Widget::Over);      //Widget线程为主线程 不需要Qt::DirectConnection标志。
    connect(mMake, &MakeBlurImage::getProcess, this, &Widget::updatePDialog);

    mMThread->start();
    toMake(0);
}

Widget::~Widget()
{
    mMThread->quit();
    mMThread->wait();
}

void Widget::updatePDialog(int range, int value)
{
    if(mProgress->isHidden())
        mProgress->show();
    mProgress->setRange(0,range);
    mProgress->setValue(value);
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(QRectF(0,0,this->width(),this->height()),mImage);
}

void Widget::Over(const QImage &img)
{
    mImage = img;
    mSlider->setEnabled(true);
    mProgress->hide();
    update();
}

void Widget::toMake(int lever)
{
    mMake->setImage(":/test.png");
    mSlider->setDisabled(true);
    emit mitMake(lever);
}
