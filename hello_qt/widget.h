#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QPushButton *mButton;
    QLabel *mHelloLabel;
};

#endif // WIDGET_H
