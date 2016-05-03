#include "widget.h"
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <QLayout>
#include <QProgressBar>
#include <QDir>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    ,mBlurLever(0)
{
    mProgress = new QProgressBar(this);
    QHBoxLayout *alayout = new QHBoxLayout;
    alayout->addStretch(1);
    QVBoxLayout *blayout = new QVBoxLayout;
    for(int i = N-1; i>=0; i--){
        mButtons[i] = new QPushButton(QString::number(i), this);
        mButtons[i]->setAttribute(Qt::WA_TranslucentBackground, true);
        connect(mButtons[i], &QPushButton::clicked, this, &Widget::buttonOn);
        blayout->addWidget(mButtons[i]);
    }
    alayout->addLayout(blayout);
    QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->addLayout(alayout);
    mLayout->addWidget(mProgress);
    this->setLayout(mLayout);
    this->setGeometry(100,100, 640,480);

    mMThread = new QThread(this);
    mMake = new MakeBlurImage;
    mMake->moveToThread(mMThread);
    connect(mMThread, &QThread::destroyed, mMake, &QObject::deleteLater);

    connect(this, &Widget::sliderValueChanged, this, &Widget::toMake);
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

void Widget::buttonOn()
{
    QPushButton *tButton = qobject_cast<QPushButton*>(this->sender());
    emit sliderValueChanged(tButton->text().toInt());
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(QRectF(0,0,this->width(),this->height()),mImage);
}

void Widget::setButtonsDisable()
{
    for(int i = 0; i<N; i++){
        mButtons[i]->setDisabled(true);
    }
}

void Widget::setButtonsEnable()
{
    for(int i = 0; i<N; i++){
        mButtons[i]->setEnabled(true);
    }
}

void Widget::Over(const QImage &img)
{
    mImage = img;
    mProgress->hide();
    setButtonsEnable();
    update();
}

void Widget::toMake(int lever)
{
    QString filePath = ":/test.png";
    QDir dir("./");
    QFileInfoList files = dir.entryInfoList();
    foreach (QFileInfo fileInfo, files) {
        if(fileInfo.suffix().toUpper() == "PNG" || fileInfo.suffix().toUpper() == "JPG"){
            filePath = fileInfo.filePath();
            break;
        }
    }
    mMake->setImage(filePath);
    setButtonsDisable();
    emit mitMake(lever);
}
