#ifndef QXXTEA_H
#define QXXTEA_H
#include <QObject>
#include <QByteArray>

class QXXTea
{
public:
    static QByteArray encrypt(const QByteArray &src, const QString &key);
    static QByteArray decrypt(const QByteArray &src, const QString &key);
};

#endif // QXXTEA_H
