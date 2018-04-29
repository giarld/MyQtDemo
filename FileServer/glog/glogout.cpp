#include "glogout.h"
#include <QMutexLocker>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QDir>

GLogOut::GLogOut(QObject *parent) : QObject(parent)
{

}

void GLogOut::setLogPath(const QString &path)
{
    QDir dir(path);
    if(!dir.exists()) {
        dir.mkpath(path);
    }
    mLogPath = path;
}

void GLogOut::putLog(int type, const QString &msg)
{
    QMutexLocker locker(&mMutex);

    QTextStream out(stderr, QIODevice::WriteOnly);
    out.setCodec("UTF-8");
    QString date = QDateTime::currentDateTime().toString("hh:mm:ss-zzz");
    QString outText = QString("[%1] ").arg(date);

    switch (type) {
    case LogType::Debug:
#ifndef QT_NO_DEBUG
        outText += "D ";
#endif
        break;
    case LogType::Info:
    default:
        outText += "I ";
        break;
    case LogType::Warning:
        outText += "W ";
        break;
    case LogType::Error:
        outText += "E ";
        break;
    }
    out << outText << msg << "\n";

    if (mLogPath != "") {
        QDir dir(mLogPath);
        if(dir.exists()) {
            QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".log";
            QString path = dir.absoluteFilePath(fileName);
            QFile file(path);
            if(file.open(QIODevice::Append)) {
                QTextStream outFile(&file);
                outFile << outText << msg << "\n";
            }
            file.close();
        }
    }
}
