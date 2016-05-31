#include "widget.h"
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    QLabel label;       //label也是继承于QWidget 所以可以单独成窗口被显示
    label.setText("<h1>Hello </h1><font color = red size = 20>Qt</font>");      //支持HTML风格
    label.show();

    return a.exec();
}
