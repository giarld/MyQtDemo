#include "fileserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QBuffer>
#include <glog.h>
#include <QFile>
#include <QDir>
#include <QFileInfo>

uchar FileServer::MSG_HEAD = 0x02;
uchar FileServer::MSG_END = 0x03;
qint64 FileServer::BlockSize = 2048;

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

void FileServer::setSearchPath(const QString &path)
{
    mSearchPath = path;
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

    uchar head;
    in >> head;
    if (head == MSG_HEAD) {
        uint code;
        uint key;
        in >> code >> key;
        if (code == SUCCESS) {
            if (key == uint(MSG_OTHER)) {

            }
            else if (key == uint(MSG_STR)) {
                QByteArray msgData;
                in >> msgData;
                emit receiveMessage(client->objectName(), QString(msgData));
            }
            else if (key == uint(MSG_GET_FILE_LIST)) {
                sendFileList(client->objectName());
            }
            else if (key == uint(MSG_GET_FILE_BLOCK)) {
                DataBlockRsp rsp;
                in >> rsp;
                sendFileBlock(client->objectName(), rsp);
            }
        }
        else {
            handleError(code);
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
    out << MSG_HEAD << (int)SUCCESS << (uint) key << data << MSG_END;
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

void FileServer::sendError(QTcpSocket *client, FileServer::MSG_KEY key, FileServer::ERROR_CODE error)
{
    QByteArray msgData;
    QBuffer buffer(&msgData);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << MSG_HEAD << (uint)key << (uint)error << MSG_END;
    buffer.close();
    client->write(msgData);
}

void FileServer::sendFileList(const QString &clientId)
{
    ClientNode node = getClientNode(clientId);
    if (!node.client) {
        return;
    }
    QDir dir(mSearchPath);

    if (!dir.exists()) {
        gLogError("sendFileList dir not exists");
        sendError(node.client, MSG_GET_FILE_LIST, ERROR_DIR_NEXISTS);
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList files = dir.entryInfoList();
    QString msgs;
    foreach (QFileInfo info, files) {
        msgs.append(info.fileName());
        msgs.append(",");
    }
    if (msgs.endsWith(",")) {
        msgs = msgs.mid(0, msgs.length() - 1);
    }
    QByteArray data;
    data.append(msgs.toUtf8());
    sendMessage(node.client, MSG_GET_FILE_LIST, data);
    emit putMessage("sendFileList success");
}

void FileServer::sendFileBlock(const QString &clientId, const DataBlockRsp &rsp)
{
    ClientNode node = getClientNode(clientId);
    if (!node.client) {
        return;
    }
    if (mSearchPath == "") {
        sendError(node.client, MSG_GET_FILE_LIST, ERROR_DIR_NEXISTS);
        return;
    }
    if (rsp.fileName == "") {
        sendError(node.client, MSG_GET_FILE_BLOCK, ERROR_FILE_NEXISTS);
        return;
    }
    QString path = getFilePath(rsp.fileName);
    if (path == "") {
        sendError(node.client, MSG_GET_FILE_BLOCK, ERROR_FILE_NEXISTS);
        return;
    }
    QFile file(path);
    if (!file.exists()) {
        sendError(node.client, MSG_GET_FILE_BLOCK, ERROR_FILE_NEXISTS);
        return;
    }
    if (file.open(QIODevice::ReadOnly)) {
        file.seek(rsp.begin);
        char *data = new char[BlockSize + 1];
        qint64 size = file.read(data, BlockSize);
        if (size < 0) {
            delete[] data;
            QByteArray msg;
            QBuffer buffer(&msg);
            buffer.open(QIODevice::WriteOnly);
            QDataStream out(&buffer);
            QByteArray fnByte;
            fnByte.append(rsp.fileName);
            out << fnByte << file.size() << QByteArray();
            buffer.close();
            sendMessage(node.client, MSG_GET_FILE_BLOCK, msg);
        }
        else {
            QByteArray bytes(data, size);
            delete[] data;
            QByteArray msg;
            QBuffer buffer(&msg);
            buffer.open(QIODevice::WriteOnly);
            QDataStream out(&buffer);
            QByteArray fnByte;
            fnByte.append(rsp.fileName);
            out << fnByte << rsp.begin + size << bytes;
            buffer.close();
            sendMessage(node.client, MSG_GET_FILE_BLOCK, msg);
        }
        file.close();
    }
}

//void FileServer::sendFile(const QString &clientId, const QString &filePath)
//{
//    if (filePath == "")
//        return;
//    ClientNode node = getClientNode(clientId);
//    if (node.client) {
//        node.inSendFile = true;
//        node.filePath = filePath;
//        node.position = 0;
//        node.lastPosition = 0;
//        QFile file(node.filePath);
//        if (file.exists()) {
//            QString fileName = file.fileName();
//            QByteArray msg;
//            QBuffer buffer(&msg);
//            buffer.open(QIODevice::WriteOnly);
//            QDataStream out(&buffer);
//            out << fileName.toUtf8();
//            buffer.close();
//            sendMessage(node.client, MSG_FILE_READY, msg);
//        }
//    }
//}

QString FileServer::genKey()
{
    return QString("client_%1").arg(mKeyIndex++);
}

QString FileServer::getFilePath(const QString &fileName)
{
    QDir dir(mSearchPath);
    if (!dir.exists()) {
        return "";
    }
    return dir.absoluteFilePath(fileName);
}

void FileServer::handleError(uint error)
{

}

QDataStream &operator <<(QDataStream &out, const DataBlockRsp &data)
{
    QByteArray fnByte;
    fnByte.append(data.fileName.toUtf8());
    out << fnByte << data.begin;
    return out;
}

QDataStream &operator >>(QDataStream &in, DataBlockRsp &data)
{
    QByteArray fnByte;
    in >> fnByte >> data.begin;
    data.fileName = QString(fnByte);
    return in;
}
