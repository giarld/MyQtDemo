#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <glog.h>
#include <QHostAddress>
#include <QByteArray>
#include <QBuffer>
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
        QString text = ui->msgEdit->text();
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        QDataStream out(&buffer);
        out << FileServer::MSG_HEAD << FileServer::MSG_STR << text.toUtf8() << FileServer::MSG_END;
        buffer.close();
        mSocket->write(data);
    });

    connect(mSocket, &QTcpSocket::readyRead, [this]() {
        QByteArray data = mSocket->readAll();
        QBuffer buffer(&data);
        buffer.open(QIODevice::ReadOnly);
        QDataStream in(&buffer);

        uchar head = 0;
        in >> head;
        if (head == FileServer::MSG_HEAD) {
            uint key = 0;
            in >> key;
            if (key == uint(FileServer::MSG_STR)) {
                QByteArray sd;
                in >> sd;
                ui->msgText->appendPlainText("Server: " + QString(sd));
            }
            else if (key == uint(FileServer::MSG_FILE_READY)) {
                QByteArray msg;
                in >> msg;
                QBuffer msgBuffer(&msg);
                msgBuffer.open(QIODevice::ReadOnly);
                QDataStream in(&msgBuffer);
                QByteArray fileNameByte;
                in >> fileNameByte;
                buffer.close();
                ui->msgText->appendPlainText("Server: Ready send file = " + QString(fileNameByte));
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
