#include "qxxtea.h"
#include "xxtea.h"

QByteArray QXXTea::encrypt(const QByteArray &src, const QString &key)
{
    xxtea_long len;
    char *data = (char *) xxtea_encrypt((uchar *)src.data(), src.length(), (uchar *)key.toLatin1().data(), key.length(), &len);
    QByteArray tar(data, (int)len);
    delete[] data;
    return tar;
}

QByteArray QXXTea::decrypt(const QByteArray &src, const QString &key)
{
    xxtea_long len;
    char *data = (char *) xxtea_decrypt((uchar *)src.data(), src.length(), (uchar *)key.toLatin1().data(), key.length(), &len);
    QByteArray tar(data, (int)len);
    delete[] data;
    return tar;
}
