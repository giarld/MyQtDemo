#include "serverwidget.h"
#include "clientwidget.h"
#include <QApplication>
#include <glog.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GLog::create();

    ServerWidget w;
    w.show();

    ClientWidget cw;
    cw.show();

    int ecode = a.exec();
    GLog::destroy();
    return ecode;
}
