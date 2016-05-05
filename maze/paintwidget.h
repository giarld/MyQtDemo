#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QList>

struct Node
{
    Node(int x,int y):x(x),y(y){}
    Node():x(0),y(0){}
    int x;
    int y;
    int ox,oy;
};

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);


protected:
    void paintEvent(QPaintEvent *);

signals:
    void updateTitle(const QString &text);

public slots:
    void setPSize(const QPoint &p);
    void calFs();
    void clearFs();
    void save2Img();

    void moveS(int f);

private:
    void makeMare();
private:
    QPoint pSize;

    QList<Node> ruList;
    QList<Node> xList;
};

#endif // PAINTWIDGET_H
