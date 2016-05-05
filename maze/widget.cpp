#include "widget.h"
#include "paintwidget.h"
#include <editdialog.h>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>

Widget::Widget(QMainWindow *parent)
    : QMainWindow(parent)
    ,dialog(Q_NULLPTR)
{
#ifndef __ANDROID__
    int widget = 640;
    int height = 480;

    this->setGeometry((QApplication::desktop()->width() - widget)/2, (QApplication::desktop()->height() - height)/2,widget,height);
#endif
    paintW = new PaintWidget(this);
    this->setCentralWidget(paintW);

    initMenu();
    connect(this,SIGNAL(updatePW(QPoint)),paintW,SLOT(setPSize(QPoint)));
    connect(paintW,SIGNAL(updateTitle(QString)),this,SLOT(setWindowTitle(QString)));

    connect(this,SIGNAL(keyMove(int)),paintW,SLOT(moveS(int)));

    emit updatePW(QPoint(16, 16));
//    emit calFsAction->trigger();
}

Widget::~Widget()
{
    if(dialog)
        delete dialog;
}

void Widget::initMenu()
{
    menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);

    startMenu = new QMenu(tr("开始"),this);
    menuBar->addMenu(startMenu);

    startAction = new QAction(tr("生成"),this);
    startMenu->addAction(startAction);

    calFsAction = new QAction(tr("计算线路"),this);
    startMenu->addAction(calFsAction);

    clearFsAction = new QAction(tr("清除路线"),this);
    startMenu->addAction(clearFsAction);

    saveImgAction = new QAction(tr("保存为图片"),this);
    startMenu->addAction(saveImgAction);

    closeAction = new QAction(tr("Exit"), this);
    startMenu->addAction(closeAction);

    connect(this->startAction,SIGNAL(triggered(bool)),this,SLOT(showStartDialog()));
    connect(this->calFsAction,SIGNAL(triggered(bool)),paintW,SLOT(calFs()));
    connect(this->clearFsAction,SIGNAL(triggered(bool)),paintW,SLOT(clearFs()));
    connect(this->saveImgAction,SIGNAL(triggered(bool)),paintW,SLOT(save2Img()));
    connect(this->closeAction,SIGNAL(triggered(bool)),this,SLOT(close()));
}

void Widget::paintEvent(QPaintEvent *e)
{
    QMainWindow::paintEvent(e);
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    int f=0;
    if(e->key() == Qt::Key_W){
        f = 4;
    }
    else if(e->key() == Qt::Key_S){
        f = 3;
    }
    else if(e->key() == Qt::Key_A){
        f = 2;
    }
    else if(e->key() == Qt::Key_D){
        f = 1;
    }
    emit keyMove(f);
}

void Widget::showStartDialog()
{
    if(dialog == Q_NULLPTR)
        dialog = new EditDialog(this);
    int r = dialog->exec();
    if(r == EditDialog::Accepted){
        emit updatePW(dialog->getData());
    }
}
