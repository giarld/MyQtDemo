#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();

protected:
    void closeEvent(QCloseEvent *event);

signals:

public slots:

private:
    void requestFile(const QString &fileName, qint64 begin);

private:
    Ui::ClientWidget *ui;
    QTcpSocket *mSocket;
};

#endif // CLIENTWIDGET_H
