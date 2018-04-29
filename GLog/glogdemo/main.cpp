#include "widget.h"
#include <QApplication>
#include <glog.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GLog::create();

    Widget w;
    w.show();

    return a.exec();
    GLog::destroy();
}
