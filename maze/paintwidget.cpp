#include "paintwidget.h"
#include <QPainter>
#include <QDateTime>
#include <QQueue>
#include <QThread>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>

int sx = 1;
int sy = 1;
int ex = 1;
int ey = 1;

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    pSize = QPoint(64,45);
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

void PaintWidget::setPSize(const QPoint &p)
{
    int x = int((p.x()/2)*2);
    int y = int((p.y()/2)*2);
    pSize.setX(x);
    pSize.setY(y);

    makeMare();
    this->update();
    QString text = tr("迷宫大小：%1 x %2").arg(x).arg(y);
    emit updateTitle(text);
}

int run[][2]={
    {1,0},{-1,0},{0,1},{0,-1}
};
const int N = 500;
bool flag[N][N];            //是否为路
bool isChk[N][N];           //是否被加入墙链表
int uset[N][N]; //并查集
int tlSet;
void PaintWidget::makeMare()
{
    /*
1、让迷宫全是墙
2、选一个格作为迷宫的通路，然后把它的邻墙放入列表
3、当列表里还有墙时
——1、从列表里随机选一个墙，如果它对面的格子不是迷宫的通路
————1、把墙打通，让对面的格子成为迷宫的通路
————2、把那个格子的邻墙加入列表
——2、如果对面的格子已经是通路了，那就从列表里移除这面墙
*/
    ruList.clear();
    xList.clear();
    memset(flag,0,sizeof(flag));
    memset(isChk,0,sizeof(isChk));
    int index = 0;
    for(int i=0;i<pSize.x();i++){
        for(int j=0;j<pSize.y();j++){
            uset[i][j] = index;
            index++;
        }
    }


    sx = int((qrand()%pSize.x())/2)*2;
    sy = int((qrand()%pSize.y())/2)*2;

    QList<Node> qList;

    flag[sx][sy] = 1;
    for(int i=0;i<4;i++){
        Node qq;
        qq.x = sx+run[i][0];
        qq.y = sy+run[i][1];
        if(qq.x<=0 || qq.y<=0 || qq.x>=pSize.x() || qq.y>=pSize.y())
            continue;
        qq.ox = sx;
        qq.oy = sy;
        isChk[qq.x][qq.y] = 1;
        qList.push_back(qq);
    }

    ruList.push_back(Node(sx,sy));
    tlSet = uset[sx][sy];
    while(qList.isEmpty() == false){
        int n = qrand()%qList.size();
        Node q = qList[n];
        qList.removeAt(n);
        int nx = (q.x - q.ox)+q.x;
        int ny = (q.y - q.oy)+q.y;
        if(nx>=0 && ny>=0 && nx<pSize.x() && ny<pSize.y()){
            if(uset[nx][ny] != tlSet){
                uset[nx][ny] = uset[q.x][q.y] = tlSet;
                flag[nx][ny] = flag[q.x][q.y] = 1;
                ruList.push_back(Node(nx,ny));
                ruList.push_back(q);
                for(int i=0;i<4;i++){
                    Node qq;
                    qq.x = nx+run[i][0];
                    qq.y = ny+run[i][1];
                    if(qq.x<0 || qq.y<0 || qq.x>=pSize.x() || qq.y>=pSize.y() || isChk[qq.x][qq.y])
                        continue;
                    qq.ox = nx;
                    qq.oy = ny;
                    isChk[qq.x][qq.y] = 1;
                    qList.push_back(qq);
                }
            }
        }
    }
    sx = 0;
    sy = 0;
    ex = pSize.x()-2;
    ey = pSize.y()-2;
    ruList.push_back(Node(sx,sy));
    ruList.push_back(Node(ex,ey));
}

struct PNode
{
    int x,y;
    PNode *last;
};

bool dest[N][N];           //是否被加入墙链表
void PaintWidget::calFs()
{
    xList.clear();
    memset(dest,0,sizeof(dest));
    foreach (Node n, ruList) {
        dest[n.x][n.y] = 1;
    }
    PNode *end = NULL;
    QQueue<PNode*> qu;
    QList<PNode*> brash;
    PNode *n = new PNode;
    brash.push_back(n);
    n->x = sx;
    n->y = sy;
    n->last=NULL;
    qu.push_back(n);
    dest[n->x][n->y] = 0;
    int count = 0;
    while(!qu.empty()){
        PNode *t = qu.front();
        qu.pop_front();
        if(t->x == ex && t->y == ey){
            end = t;
            break;
        }
        for(int i=0;i<4;i++){
            int xx = t->x+run[i][0];
            int yy = t->y+run[i][1];
            if(xx>=0 && yy>=0 && xx<pSize.x() && yy<pSize.y() && dest[xx][yy]==1){
                dest[xx][yy] = 0;
                PNode *ne = new PNode;
                brash.push_back(ne);
                ne->x = xx;
                ne->y = yy;
                ne->last = t;
                qu.push_back(ne);
                count++;
            }
        }
    }

    while(end != NULL){
        xList.push_back(Node(end->x,end->y));
        end = end->last;
    }
    foreach (PNode *p, brash) {
        delete p;
    }
    update();
}

void PaintWidget::clearFs()
{
    xList.clear();
    update();
}

void PaintWidget::save2Img()
{
    int bw = 10;
    int bh = 10;
    QPixmap img(bw*(pSize.x()+1),bh*(pSize.y()+1));
    img.fill(Qt::black);
    QPainter painter(&img);

    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);

    bool isD = false;
    foreach (Node n, ruList) {
        painter.drawRect(QRectF(n.x*bw+bw,n.y*bh+bh,bw,bh));
        isD = true;
    }

    painter.setPen(Qt::green);
    painter.setBrush(Qt::green);
    foreach (Node p, xList) {
        painter.drawRect(QRectF(p.x*bw+bw+bw*0.1,p.y*bh+bh+bh*0.1,bw*0.8,bh*0.8));
    }
    if(isD){
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.drawRect(sx*bw+bw,sy*bh+bh,bw,bh);

        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);
        painter.drawRect(ex*bw+bw,ey*bh+bh,bw,bh);
    }
    painter.end();
#ifdef __ANDROID__
    QString filename = "/sdcard/maze_" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz") + ".png";
#else
    QString filename = QFileDialog::getSaveFileName(this,tr("保存为图片"),tr("./untitled.png"),tr("Images (*.png *.xpm *.jpg)"));
#endif
    if(filename!=""){
        QFileInfo info(filename);
        img.save(filename,info.suffix().toUpper().toLatin1().data());
        QMessageBox::information(this,tr("提示"),tr("已保存为\"%1\"").arg(filename));
    }
    else{
        QMessageBox::warning(this,tr("提示"),tr("保存失败"));
    }
}

void PaintWidget::moveS(int f)
{
    if(f == 0)
        return ;
    int xx=sx;
    int yy=sy;
    switch(f){
    case 1:
        xx+=1;
        break;
    case 2:
        xx-=1;
        break;
    case 3:
        yy+=1;
        break;
    case 4:
        yy-=1;
        break;
    };
    if(uset[xx][yy] == tlSet){
        sx=xx;
        sy=yy;
        update();
    }
    if(sx == ex && sy == ey){
        sx=0;sy=0;
        QMessageBox::information(this,tr("胜利"),tr("恭喜你走出了迷宫"));
        calFs();
    }
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    float bw = float(this->width()*1.0f/(pSize.x()+1));
    float bh = float(this->height()*1.0f/(pSize.y()+1));

    painter.drawRect(0,0,this->width(),this->height());
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);

    bool isD = false;
    foreach (Node n, ruList) {
        painter.drawRect(QRectF(n.x*bw+bw,n.y*bh+bh,bw,bh));
        isD = true;
    }

    painter.setPen(Qt::green);
    painter.setBrush(Qt::green);
    foreach (Node p, xList) {
        painter.drawRect(QRectF(p.x*bw+bw+bw*0.1,p.y*bh+bh+bh*0.1,bw*0.8,bh*0.8));
    }
    if(isD){
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.drawRect(sx*bw+bw,sy*bh+bh,bw,bh);

        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);
        painter.drawRect(ex*bw+bw,ey*bh+bh,bw,bh);
    }
    painter.end();
}

