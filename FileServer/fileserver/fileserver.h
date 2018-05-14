#ifndef FILESERVER_H
#define FILESERVER_H

#include <fileserver_global.h>
#include <QObject>
#include <QHash>
#include <QAbstractSocket>
#include <QMutex>
#include <QDataStream>

class QTcpSocket;
class QTcpServer;

struct DataBlockRsp
{
    QString fileName;
    qint64 begin;
};
QDataStream &operator <<(QDataStream &out, const DataBlockRsp &data);
QDataStream &operator >>(QDataStream &in, DataBlockRsp &data);

class FILESERVERSHARED_EXPORT FileServer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief The MSG_KEY enum
     * 消息码
     */
    enum MSG_KEY
    {
        MSG_NONE = 0x0,
        MSG_STR = 0xB001,        //纯文本
        MSG_OTHER,          //其他
        MSG_GET_FILE_LIST,  //请求获取文件列表
        MSG_GET_FILE_BLOCK, //请求获取文件块
    };

    /**
     * @brief The FILE_ERROR_CODE enum
     * 文件传输错误处理码
     */
    enum ERROR_CODE
    {
        SUCCESS = 1000,
        ERROR_POS,     //客户端，包顺序不对
        ERROR_SIZE,             //客户端，包大小不对
        ERROR_FILE_WRITE,       //客户端，写文件失败
        ERROR_FILE_READ,      //服务端，读取文件失败
        ERROR_DIR_NEXISTS,      //目录不存在
        ERROR_FILE_NEXISTS,     //文件不存在
    };

    struct ClientNode
    {
        ClientNode()
            : client(nullptr) {}

        ClientNode(const ClientNode &node) {
            this->client = node.client;
        }

        ClientNode(QTcpSocket *ts)
            : client(ts) {}

        QTcpSocket *client;
    };

    static uchar MSG_HEAD;
    static uchar MSG_END;

public:
    FileServer(QObject *parent = 0);
    ~FileServer();

    void start(int port);
    void stop();

    void setSearchPath(const QString &path);

    ClientNode getClientNode(const QString &id);
    QTcpSocket *getClient(const QString &id);

private slots:
    void onNewConnection();
    void onServerError(QAbstractSocket::SocketError socketError);
    void onDisConnect();
    void readMessage();

public slots:
    /**
     * 消息体结构：MSG_HEAD + MSG_KEY + DATA_LEN + DATA + MSG_END
     */
    void sendMessage(QTcpSocket *client, MSG_KEY key, const QByteArray &data);
    void sendStringMsg(QTcpSocket *client, const QString msg);

    void sendStringMsgToAll(const QString &msg);

    void sendError(QTcpSocket *client, MSG_KEY key, ERROR_CODE error);

signals:
    void newConnection(const QString &clientId);
    void clientDisconnected(const QHostAddress &address);
    void receiveMessage(const QString &clientId, const QString &msg);
    void putMessage(const QString &msg);

private:
    void sendFileList(const QString &clientId);
    void sendFileBlock(const QString &clientId, const DataBlockRsp &rsp);

private:
    QString genKey();
    QString getFilePath(const QString &fileName);

    void handleError(uint error);

private:
    QTcpServer *mServer;
    QHash<QString, ClientNode> mClients;

    ulong mKeyIndex;

    QMutex mLock;

    QString mSearchPath;

    static qint64 BlockSize;
};

#endif // FILESERVER_H
