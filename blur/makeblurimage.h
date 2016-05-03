#ifndef MAKEBLURIMAGE_H
#define MAKEBLURIMAGE_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QImage>

class MakeBlurImage : public QObject
{
    Q_OBJECT
public:
    explicit MakeBlurImage(QObject *parent = 0);
    void setImage(const QString& filename);
signals:
    void makeOver(const QImage& img);
    void getProcess(int range, int value);
public slots:
    void makeBlur(int lever);
private:
    QImage mImage;
    QMutex mMutex;
};

#endif // MAKEBLURIMAGE_H
