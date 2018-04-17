#include "widget.h"
#include <QProcess>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mSubProcess = new QProcess(this);
    mSubProcess->start("../../../subProgress.app/Contents/MacOS/subProgress",
                       QStringList() << "0" << "0" << "640" << "480");
}

Widget::~Widget()
{
    mSubProcess->close();
}
