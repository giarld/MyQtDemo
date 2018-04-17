#include "widget.h"
#include <QApplication>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QStringList cmds = QApplication::arguments();

    if(cmds.size() == 5) {
        int x = cmds[1].toInt();
        int y = cmds.at(2).toInt();
        int w = cmds.at(3).toInt();
        int h = cmds.at(4).toInt();
        setGeometry(x, y, w, h);
    }

    QLabel *label = new QLabel(this);
    label->setText(QApplication::arguments().join("\n"));
}

Widget::~Widget()
{

}
