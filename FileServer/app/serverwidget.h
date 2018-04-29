#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class FileServer;

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::ServerWidget *ui;

    FileServer *mFileServer;
};

#endif // WIDGET_H
