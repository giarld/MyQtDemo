#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    HttpServer(QObject *parent = 0);

private slots:
    void onNewConnection();
    void onServerError(QAbstractSocket::SocketError socketError);
    void onDisConnect();
    void readMessage();

private:
    QTcpServer *mServer;
};

#endif // HTTPSERVER_H
