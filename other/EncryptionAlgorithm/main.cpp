#include <QCoreApplication>
#include "tea/qxxtea.h"
#include "QDebug"
#include <QFile>
#include <QDataStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString key = "c128cb3171734e1dab3fb9da64c6db04";

    QString testPath = "/Users/gxin/Downloads/";
    QFile inFile(testPath + "test.png");
    if(inFile.open(QIODevice::ReadOnly)) {
        qDebug("读取源文件..");
        QByteArray bytes = inFile.readAll();
        qDebug() << "读取完成 size =" << bytes.size();
        qDebug() << "开始加密";
        QByteArray encodeBytes = QXXTea::encrypt(bytes, key);
        qDebug() << "加密完成 size =" << encodeBytes.size();
        inFile.close();

        QFile outFile(testPath + "encode.png");
        if(outFile.open(QIODevice::WriteOnly)) {
            qDebug() << "开始写文件";
            outFile.write(encodeBytes.data(), encodeBytes.length());
            outFile.close();
            qDebug() << "写入完成";
        }

        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<";
        qDebug() << "开始解密";
        if(outFile.open(QIODevice::ReadOnly)) {
            QByteArray eBytes = outFile.readAll();
            qDebug() << "读取到文件 size =" << eBytes.size();
            QByteArray decodeByets = QXXTea::decrypt(eBytes, key);
            qDebug() << "解密完成 size =" << decodeByets.size();
            outFile.close();

            QFile deOutFile(testPath + "decode.png");
            if(deOutFile.open(QIODevice::WriteOnly)) {
                deOutFile.write(decodeByets.data(), decodeByets.length());
                deOutFile.close();
                qDebug() << "解密导出完成";
            }
        }
    }

    return a.exec();
}
