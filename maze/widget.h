#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>

class PaintWidget;
class EditDialog;

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QMainWindow *parent = 0);
    ~Widget();

    void initMenu();

protected:
    void paintEvent(QPaintEvent*e);
    void keyPressEvent(QKeyEvent *e);

signals:
    void updatePW(const QPoint &p);
    void keyMove(int f);

public slots:
    void showStartDialog();

private:
    PaintWidget *paintW;

    QMenuBar* menuBar;
    QMenu* startMenu;
    QAction* startAction;
    QAction* calFsAction;
    QAction* clearFsAction;
    QAction* saveImgAction;
    QAction* closeAction;

    EditDialog *dialog;
};

#endif // WIDGET_H
