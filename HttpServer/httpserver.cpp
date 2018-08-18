#include "httpserver.h"

HttpServer::HttpServer(QObject *parent)
    : QObject(parent)
{
    mServer = new QTcpServer(this);

    connect(mServer, &QTcpServer::newConnection, this, &HttpServer::onNewConnection);
    connect(mServer, &QTcpServer::acceptError, this, &HttpServer::onServerError);

}
