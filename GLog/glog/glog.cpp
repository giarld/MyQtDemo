#include "glog.h"
#include "glogout.h"

GLog *GLog::clogInstance = nullptr;

GLog::GLog()
    : QObject(0)
{
    mLogOut = new GLogOut;
    mOutThread = new QThread(this);
    mLogOut->moveToThread(mOutThread);
    mOutThread->start();
    connect(mOutThread, &QThread::finished, mLogOut, &GLogOut::deleteLater);
    connect(this, &GLog::logPut, mLogOut, &GLogOut::putLog);
}

GLog::~GLog()
{
    mOutThread->quit();
    mOutThread->wait();
    mOutThread->deleteLater();
    disconnect(this, &GLog::logPut, mLogOut, &GLogOut::putLog);
}

void GLog::create()
{
    if (!clogInstance) {
        clogInstance = new GLog;
    }
}

void GLog::destroy()
{
    if(clogInstance) {
        delete clogInstance;
        clogInstance = nullptr;
    }
}

GLog *GLog::getInstance()
{
    return clogInstance;
}

void GLog::setLogPath(const QString &path)
{
    mLogOut->setLogPath(path);
}

void GLog::debug(const char *file, int line, const char *function, const QString &log)
{
#ifndef QT_NO_DEBUG
    if(GLog::getInstance()) {
        QString msg = QString("%1 %2 %3 %4").arg(file).arg(line).arg(function).arg(log);
        emit GLog::getInstance()->logPut(GLogOut::Debug, msg);
    }
#endif
}

void GLog::info(const QString &log)
{
    if(GLog::getInstance())
        emit GLog::getInstance()->logPut(GLogOut::Info, log);
}

void GLog::warn(const QString &log)
{
    if(GLog::getInstance())
        emit GLog::getInstance()->logPut(GLogOut::Warning, log);
}

void GLog::error(const QString &log)
{
    if(GLog::getInstance())
        emit GLog::getInstance()->logPut(GLogOut::Error, log);
}
