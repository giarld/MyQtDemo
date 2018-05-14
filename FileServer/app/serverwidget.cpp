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
    mFileServer->setSearchPath("./test/");

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
        mFileServer->sendStringMsgToAll(ui->msgEdit->text());
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
