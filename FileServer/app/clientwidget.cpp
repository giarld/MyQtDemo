#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <glog.h>
#include <QHostAddress>
#include <QByteArray>
#include <QBuffer>
#include <QFile>
#include <QDataStream>
#include <fileserver.h>

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWidget)
{
    gLogDebug("ClientWidget created");
    ui->setupUi(this);
    setWindowTitle("客户端");

    mSocket = new QTcpSocket(this);
    connect(mSocket, &QTcpSocket::connected, [this] () {
        ui->msgText->appendPlainText(QString("Connected to %1:%2")
                                     .arg(mSocket->peerAddress().toString())
                                     .arg(mSocket->peerPort()));
        ui->linkButton->setVisible(false);
    });
    connect(mSocket, &QTcpSocket::disconnected, [this]() {
        ui->msgText->appendPlainText("disconnected");
    });

    connect(ui->linkButton, &QPushButton::clicked, [this]() {
        mSocket->abort();
        mSocket->connectToHost(QHostAddress::LocalHost, 8081);
    });
    connect(ui->sendButton, &QPushButton::clicked, [this]() {
//        QString text = ui->msgEdit->text();
//        QByteArray data;
//        QBuffer buffer(&data);
//        buffer.open(QIODevice::WriteOnly);
//        QDataStream out(&buffer);
//        out << FileServer::MSG_HEAD << (uint)FileServer::SUCCESS << (uint)FileServer::MSG_STR << text.toUtf8() << FileServer::MSG_END;
//        buffer.close();
//        mSocket->write(data);

//        QByteArray data;
//        QBuffer buffer(&data);
//        buffer.open(QIODevice::WriteOnly);
//        QDataStream out(&buffer);
//        out << FileServer::MSG_HEAD << (uint)FileServer::SUCCESS << (uint)FileServer::MSG_GET_FILE_LIST << FileServer::MSG_END;
//        buffer.close();
//        mSocket->write(data);

          requestFile("big.apk", 0);
    });

    connect(mSocket, &QTcpSocket::readyRead, [this]() {
        QByteArray data = mSocket->readAll();
        if (data.isEmpty()) {
            gLogError("readMessage data is Null");
            return;
        }

        if (data.at(0) != FileServer::MSG_HEAD || data.at(data.length() - 1) != FileServer::MSG_END) {
            gLogError("readMessage data error");
            return;
        }
        QBuffer buffer(&data);
        buffer.open(QIODevice::ReadOnly);
        QDataStream in(&buffer);

        uchar head;
        in >> head;
        if (head == FileServer::MSG_HEAD) {
            uint code;
            uint key;
            in >> code >> key;
            if (key == uint(FileServer::MSG_STR)) {
                QByteArray sd;
                in >> sd;
                ui->msgText->appendPlainText("Server: " + QString(sd));
            }
            else if (key == uint(FileServer::MSG_GET_FILE_LIST)) {
                QByteArray msg;
                in >> msg;
                ui->msgText->appendPlainText("Server: Ready send file = " + QString(msg));
            }
            else if (key == uint(FileServer::MSG_GET_FILE_BLOCK)) {
                QByteArray msg;
                in >> msg;
                QBuffer fbuffer(&msg);
                fbuffer.open(QIODevice::ReadOnly);
                QDataStream inf(&fbuffer);
                QByteArray fnByte;
                qint64 pos;
                QByteArray fdata;
                inf >> fnByte >> pos >> fdata;
                fbuffer.close();
                QString fileName = QString(fnByte);
                QFile file(fileName);
                if (file.open(QIODevice::Append)) {
                    file.write(fdata);
                    file.close();
                    ui->msgText->appendPlainText("Server: download file = " + fileName + " pos = " + QString::number(pos));
                }
                if (fdata.size() == 2048)
                    requestFile(fileName, pos);
            }
        }

        buffer.close();
    });
}

ClientWidget::~ClientWidget()
{

}

void ClientWidget::closeEvent(QCloseEvent *event)
{
    mSocket->close();
}

void ClientWidget::requestFile(const QString &fileName, qint64 begin)
{
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    DataBlockRsp rsp;
    rsp.fileName = fileName;
    rsp.begin = begin;
    out << FileServer::MSG_HEAD << (uint)FileServer::SUCCESS << (uint)FileServer::MSG_GET_FILE_BLOCK << rsp << FileServer::MSG_END;
    buffer.close();
    mSocket->write(data);
}


