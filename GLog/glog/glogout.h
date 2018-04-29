#ifndef CLOGOUT_H
#define CLOGOUT_H

#include <QObject>
#include <QMutex>

class GLogOut : public QObject
{
    Q_OBJECT
public:
    enum LogType{
        Debug = 0x0,
        Info,
        Warning,
        Error
    };

private:
    explicit GLogOut(QObject *parent = nullptr);
    void setLogPath(const QString &path);

public slots:
    void putLog(int type, const QString &msg);

private:
    friend class GLog;
    QMutex mMutex;
    QString mLogPath;
};

#endif // CLOGOUT_H
