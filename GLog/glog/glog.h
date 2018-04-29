#ifndef CLOG_H
#define CLOG_H

#include <glog_global.h>
#include <qlogging.h>
#include <QObject>
#include <QThread>
#include "glogout.h"
#include <QDebug>

class GLOGSHARED_EXPORT GLog : public QObject
{
    Q_OBJECT
public:
    static void create();
    static void destroy();
    static GLog *getInstance();

public:
    void setLogPath(const QString &path);

public:
    static void debug(const char *file, int line, const char *function, const QString &log);
    static void info(const QString &log);
    static void warn(const QString &log);
    static void error(const QString &log);

private:
    GLog();
    ~GLog();

signals:
    void logPut(int type, const QString &msg);

private:
    static GLog *clogInstance;

    QThread *mOutThread;
    GLogOut *mLogOut;
};

#define gLogDebug(m) GLog::debug(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, m)
#define gLogInfo(i) GLog::info(i)
#define gLogWarn(w) GLog::warn(w)
#define gLogError(e) GLog::error(e)

#endif // CLOG_H
