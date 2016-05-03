#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <makeblurimage.h>
#include <QImage>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent* key);

public slots:
    void Over(const QImage& img);
signals:
    void toMake(int lever);

private:
    int mBlurLever;
    QImage mImage;
    QThread *mMThread;
    MakeBlurImage *mMake;
};

#endif // WIDGET_H
