#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *event);

public slots:
    void mouseP();

private:
    GLWidget *widget;
    QTimer *timer;
    QTimer *mouseTimer;

    int inworld;
};

#endif // MAINWINDOW_H
