#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void buttonClick();
    void replyFinished(QNetworkReply *reply);

private:
    QPushButton *mDownButton;
    QLineEdit *mURLEdit;
    QLabel  *mImgView;
    QNetworkAccessManager *mNetWorkManager;
};

#endif // WIDGET_H
