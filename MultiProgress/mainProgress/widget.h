#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QProcess;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QProcess *mSubProcess;
};

#endif // WIDGET_H
