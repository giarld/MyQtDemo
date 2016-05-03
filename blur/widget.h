#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <makeblurimage.h>
#include <QImage>
#include <QPushButton>

class QProgressBar;
#define N 6

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent*);

    void setButtonsDisable();
    void setButtonsEnable();
public slots:
    void Over(const QImage& img);
    void toMake(int lever);
    void updatePDialog(int range, int value);
    void buttonOn();

signals:
    void mitMake(int lever);
    void sliderValueChanged(int v);

private:
    int mBlurLever;
    QPushButton *mButtons[N];
    QImage mImage;
    QThread *mMThread;
    MakeBlurImage *mMake;
    QProgressBar *mProgress;
};

#endif // WIDGET_H
