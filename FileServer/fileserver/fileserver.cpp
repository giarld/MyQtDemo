#include "fileserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QBuffer>
#include <glog.h>
#include <QFile>

uchar FileServer::MSG_HEAD = 0x02;
uchar FileServer::MSG_END = 0x03;
qint64 FileServer::BlockSize = 1024;

FileServer::FileServer(QObject *parent)
    : QObject(parent)
{
    mKeyIndex = 1;

    mServer = new QTcpServer(this);

    connect(mServer, &QTcpServer::newConnection, this, &FileServer::onNewConnection);
    connect(mServer, &QTcpServer::acceptError, this, &FileServer::onServerError);
}

FileServer::~FileServer()
{
    stop();
}

void FileServer::start(int port)
{
    stop();
    if (!mServer->listen(QHostAddress::LocalHost, port)) {
        gLogDebug(QString("create server fail error = %1").arg(mServer->errorString()));
        mServer->close();
    }
    else {
        gLogDebug(QString("create server success %1").arg(mServer->serverAddress().toString()));
    }
}

void FileServer::stop()
{
    gLogDebug("FileServer stop");
    if (mServer->isListening()) {
        mServer->close();
    }

    QMutexLocker locker(&mLock);
    QHashIterator<QString, ClientNode> it(mClients);
    while (it.hasNext()) {
        it.next();
        it.value().client->deleteLater();
    }
    mClients.clear();
}

FileServer::ClientNode FileServer::getClientNode(const QString &id)
{
    QMutexLocker locker(&mLock);
    ClientNode def;
    return mClients.value(id, def);
}

QTcpSocket *FileServer::getClient(const QString &id)
{
    QMutexLocker locker(&mLock);
    if (mClients.contains(id)) {
        return mClients.value(id).client;
    }
    return nullptr;
}

void FileServer::onNewConnection()
{
    QTcpSocket *client = mServer->nextPendingConnection();
    if (client == nullptr) {
        gLogDebug("onNewConnection a null client");
        return;
    }
    gLogDebug(QString("onNewConnection %1 conneted").arg(client->peerAddress().toString()));
    QString key = genKey();
    client->setObjectName(key);
    ClientNode cn(client);

    mLock.lock();
    mClients.insert(key, cn);
    mLock.unlock();

    connect(client, &QTcpSocket::disconnected, this, &FileServer::onDisConnect);
    connect(client, &QTcpSocket::readyRead, this, &FileServer::readMessage);
    emit newConnection(client->objectName());

    sendStringMsg(client, "Hello");
}

void FileServer::onServerError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
}

void FileServer::onDisConnect()
{
    QMutexLocker locker(&mLock);
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        QString key = client->objectName();
        if (mClients.contains(key)) {
            mClients.remove(key);
        }
        emit clientDisconnected(client->peerAddress());
        client->deleteLater();
        gLogDebug(QString("disConnect client %1").arg(key));
    }
}

void FileServer::readMessage()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = client->readAll();
    if (data.isEmpty()) {
        gLogError("readMessage data is Null");
        return;
    }

    if (data.at(0) != MSG_HEAD || data.at(data.length() - 1) != MSG_END) {
        gLogError("readMessage data error");
        return;
    }
    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    uchar head = 0;
    in >> head;
    if (head == MSG_HEAD) {
        uint key = 0;
        in >> key;
        if (key == uint(MSG_OTHER)) {

        }
        else if (key == uint(MSG_STR)) {
            QByteArray msgData;
            in >> msgData;
            emit receiveMessage(client->objectName(), QString(msgData));
        }
        else if (key == uint(MSG_FILE_READY)) {
            uint ok = 0;
            in >> ok;
            if (ok) {
                emit putMessage(QString("%1 准备好接收文件").arg(client->objectName()));
                sendFileHead(client->objectName());
            }
            else {
                emit putMessage(QString("%1 拒绝的文件发送请求").arg(client->objectName()));
            }
        }
        else if (key == uint(MSG_FILE_HEAD)) {
            uint keep = 0;
            in >> keep;
            if (keep) {
                sendFileNextBlock(client->objectName());
            } else {
                endSendFile(client->objectName());
            }
        }
        else if (key == uint(MSG_FILE_BODY)) {
            uint keep = 0;
            in >> keep;
            if (keep) {
                sendFileNextBlock(client->objectName());
            } else {
                endSendFile(client->objectName());
            }
        }
        else if (key == uint(MSG_FILE_END)) {
            endSendFile(client->objectName());
        }
        else if (key == uint(MSG_FILE_ERROR)) {

        }
        else if (key == uint(MSG_FILE_RETRY)) {

        }
    }

    buffer.close();
}

void FileServer::sendMessage(QTcpSocket *client, FileServer::MSG_KEY key, const QByteArray &data)
{
    QByteArray msgData;
    QBuffer buffer(&msgData);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << MSG_HEAD << (uint) key << data << MSG_END;
    buffer.close();
    client->write(msgData);
}

void FileServer::sendStringMsg(QTcpSocket *client, const QString msg)
{
    QByteArray data;
    data.append(msg.toUtf8());
    sendMessage(client, MSG_KEY::MSG_STR, data);
}

void FileServer::sendStringMsgToAll(const QString &msg)
{
    QMutexLocker locker(&mLock);
    QHashIterator<QString, ClientNode> it(mClients);
    while (it.hasNext()) {
        it.next();
        sendStringMsg(it.value().client, msg);
    }
}

void FileServer::sendFile(const QString &clientId, const QString &filePath)
{
    if (filePath == "")
        return;
    ClientNode node = getClientNode(clientId);
    if (node.client) {
        node.inSendFile = true;
        node.filePath = filePath;
        node.position = 0;
        node.lastPosition = 0;
        QFile file(node.filePath);
        if (file.exists()) {
            QString fileName = file.fileName();
            QByteArray msg;
            QBuffer buffer(&msg);
            buffer.open(QIODevice::WriteOnly);
            QDataStream out(&buffer);
            out << fileName.toUtf8();
            buffer.close();
            sendMessage(node.client, MSG_FILE_READY, msg);
        }
    }
}

void FileServer::sendFileHead(const QString &clientId)
{
    ClientNode node = getClientNode(clientId);
    if (node.client && node.inSendFile) {
        QFile file(node.filePath);
        if (!file.exists()) {
            sendFileError(node.client, FileServer::EROOR_FILE_READ);
            endSendFile(clientId);
        }
        else if (!file.open(QIODevice::ReadOnly)) {
            sendFileError(node.client, FileServer::EROOR_FILE_READ);
            endSendFile(clientId);
        }
        else {
            QByteArray data;
            QBuffer buffer(&data);
            buffer.open(QIODevice::WriteOnly);
            QDataStream out(&buffer);
            out << file.fileName().toUtf8() << file.size();
            buffer.close();
            sendMessage(node.client, MSG_FILE_HEAD, data);
            file.close();
        }
    }
}

void FileServer::sendFileNextBlock(const QString &clientId)
{
    ClientNode node = getClientNode(clientId);
    if (node.client && node.inSendFile) {
        QFile file(node.filePath);
        if (!file.exists()) {
            sendFileError(node.client, FileServer::EROOR_FILE_READ);
            endSendFile(clientId);
        }
        else if (!file.open(QIODevice::ReadOnly)) {
            sendFileError(node.client, FileServer::EROOR_FILE_READ);
            endSendFile(clientId);
        }
        else {
            if(node.position >= file.size()) {
                QByteArray data;
                data.append(1);
                sendMessage(node.client, MSG_FILE_END, data);
            } else {
                file.seek(node.position);
                char *data = new char[BlockSize + 1];
                qint64 size = file.read(data, BlockSize);
                if (size < 0) {
                    delete[] data;
                    QByteArray msg;
                    msg.append(1);
                    sendMessage(node.client, MSG_FILE_END, msg);
                } else {
                    node.lastPosition = node.position;
                    node.position += size;
                    QByteArray bytes(data, size);
                    delete[] data;
                    QByteArray msg;
                    QBuffer buffer(&msg);
                    buffer.open(QIODevice::WriteOnly);
                    QDataStream out(&buffer);
                    out << node.lastPosition << bytes;
                    buffer.close();
                    sendMessage(node.client, MSG_FILE_BODY, msg);
                }
            }
            file.close();
        }
    }
}

void FileServer::endSendFile(const QString &clientId)
{
    ClientNode node = getClientNode(clientId);
    if (node.client) {
        node.inSendFile = false;
        node.filePath = "";
        node.position = node.lastPosition = 0;
    }
}

void FileServer::sendFileError(QTcpSocket *client, FileServer::FILE_ERROR_CODE code)
{

}

QString FileServer::genKey()
{
    return QString("client_%1").arg(mKeyIndex++);
}
