#include "widget.h"
#include <QLayout>
#include <QNetworkRequest>
#include <QByteArray>
#include <QPixmap>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QNetworkReply>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mURLEdit = new QLineEdit(this);
    mDownButton = new QPushButton(tr("download"), this);
    mImgView = new QLabel(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mURLEdit);
    layout->addWidget(mDownButton);
    layout->addWidget(mImgView);
    layout->setStretchFactor(mURLEdit, 0);
    layout->setStretchFactor(mDownButton, 0);
    layout->setStretchFactor(mImgView, 1);
    this->setLayout(layout);

    connect(mDownButton, &QPushButton::clicked, this, &Widget::buttonClick);

    mNetWorkManager = new QNetworkAccessManager(this);
    connect(mNetWorkManager, &QNetworkAccessManager::finished, this, &Widget::replyFinished);
}

Widget::~Widget()
{

}

void Widget::buttonClick()
{
    mDownButton->setDisabled(true);
    QString url = mURLEdit->text();

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    mNetWorkManager->get(request);
}

void Widget::replyFinished(QNetworkReply *reply)
{
    mDownButton->setEnabled(true);

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        mImgView->setPixmap(pixmap);

        QByteArray md5 = QCryptographicHash::hash(bytes, QCryptographicHash::Md5);
        QString filename = md5.toHex() + ".png";
        pixmap.save(filename, "PNG");
    }
    else
    {
        QMessageBox::warning(this, "error", "download image error!!");
    }
}
