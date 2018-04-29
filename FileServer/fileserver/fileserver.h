#ifndef FILESERVER_H
#define FILESERVER_H

#include <fileserver_global.h>
#include <QObject>
#include <QHash>
#include <QAbstractSocket>
#include <QMutex>

class QTcpSocket;
class QTcpServer;

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
        MSG_FILE_READY, //传输文件准备
        MSG_FILE_HEAD,  //传输文件头
        MSG_FILE_BODY,  //传输文件体
        MSG_FILE_END,   //传输文件结束
        MSG_FILE_ERROR,  //传输文件错误
        MSG_FILE_RETRY,     //对端请求重试
    };

    /**
     * @brief The FILE_ERROR_CODE enum
     * 文件传输错误处理码
     */
    enum FILE_ERROR_CODE
    {
        ERROR_POS = 0x1001,     //客户端，包顺序不对
        ERROR_SIZE,             //客户端，包大小不对
        ERROR_FILE_WRITE,       //客户端，写文件失败
        EROOR_FILE_READ,      //服务端，读取文件失败
    };

    struct ClientNode
    {
        ClientNode()
            : client(nullptr)
            , inSendFile(false)
            , filePath("")
            , position(0) {}

        ClientNode(const ClientNode &node) {
            this->client = node.client;
            this->inSendFile = node.inSendFile;
            this->filePath = node.filePath;
            this->position = node.position;
        }

        ClientNode(QTcpSocket *ts)
            : client(ts)
            , inSendFile(false)
            , filePath("")
            , position(0) {}

        QTcpSocket *client;
        bool inSendFile;
        QString filePath;
        qint64 position;
        qint64 lastPosition;
    };

    static uchar MSG_HEAD;
    static uchar MSG_END;

public:
    FileServer(QObject *parent = 0);
    ~FileServer();

    void start(int port);
    void stop();

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

    /**
     * @brief sendFile
     * 发送文件流程：
     * 1. S发送MSG_FILE_READY，包含文件名
     * 2. C回MSG_FILE_READY,如果回数据为1,则允许发送
     * 3. S发送MSG_FILE_HEAD，包含文件名+文件长度
     * 4. C回MSG_FILE_HEAD,数据1表示可以继续，0表示取消
     * 5. S开始发第一个包MSG_FILE_BODY,包含包起点+数据
     * 6. C回MSG_FILE_BODY,数据1表示可以继续，0表示取消
     * 7. 循环5->6,直到S发现发送完成，发MSG_FILE_END
     * 8. C回MSG_FILE_END，整个流程结束，S结束状态
     * @param clientId
     * @param filePath
     */
    void sendFile(const QString & clientId, const QString &filePath);

signals:
    void newConnection(const QString &clientId);
    void clientDisconnected(const QHostAddress &address);
    void receiveMessage(const QString &clientId, const QString &msg);
    void putMessage(const QString &msg);

private:
    /**
     * @brief beginSendFile 开始发送文件
     * @param clientId
     */
    void sendFileHead(const QString &clientId);
    /**
     * @brief sendFileNextBlock 发送下一个数据块
     * @param clientId
     */
    void sendFileNextBlock(const QString &clientId);

    void endSendFile(const QString &clientId);

    void sendFileError(QTcpSocket *client, FILE_ERROR_CODE code);

private:
    QString genKey();

private:
    QTcpServer *mServer;
    QHash<QString, ClientNode> mClients;

    ulong mKeyIndex;

    QMutex mLock;

    static qint64 BlockSize;
};

#endif // FILESERVER_H
