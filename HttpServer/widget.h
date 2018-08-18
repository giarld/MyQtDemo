#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class HttpServer;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    HttpServer *mHttpServer;
};

#endif // WIDGET_H
