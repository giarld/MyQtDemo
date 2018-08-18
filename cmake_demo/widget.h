//
// Created by gxin on 18-8-18.
//

#ifndef CMAKE_DEMO_WIDGET_H
#define CMAKE_DEMO_WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Widget *ui;
};


#endif //CMAKE_DEMO_WIDGET_H
