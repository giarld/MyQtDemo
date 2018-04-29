#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <fileserver.h>
#include <glog.h>
#include <QTcpSocket>
#include <QHostAddress>

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    gLogDebug("ServerWidget created");
    ui->setupUi(this);

    mFileServer = new FileServer(this);

    mFileServer->start(8081);

    this->setWindowTitle(tr("服务端"));

    connect(mFileServer, &FileServer::newConnection, [this](const QString &clientId) {
        QTcpSocket *client = mFileServer->getClient(clientId);
        if (client)
            ui->msgText->appendPlainText(QString("A new connect %1").arg(client->peerAddress().toString()));
    });
    connect(mFileServer, &FileServer::clientDisconnected, [this](const QHostAddress &address) {
        ui->msgText->appendPlainText(QString("Client %1 disconnect").arg(address.toString()));
    });
    connect(mFileServer, &FileServer::receiveMessage, [this](const QString &clientId, const QString &msg) {
        QTcpSocket *client = mFileServer->getClient(clientId);
        if (client) {
            ui->msgText->appendPlainText(
                        QString("%1: %2").arg(client->objectName()).arg(msg)
                        );
        }
    });
    connect(mFileServer, &FileServer::putMessage, [this](const QString &msg) {
        ui->msgText->appendPlainText(msg);
    });


    connect(ui->sendButton, &QPushButton::clicked, [this]() {
//        QString text = ui->msgEdit->text();
//        mFileServer->sendStringMsgToAll(text);
        mFileServer->sendFile("client_1", "/home/gxin/下载/qt-opensource-linux-x64-5.10.0.run");
    });
}

ServerWidget::~ServerWidget()
{
    delete ui;
}

void ServerWidget::closeEvent(QCloseEvent *event)
{
    mFileServer->stop();
}
