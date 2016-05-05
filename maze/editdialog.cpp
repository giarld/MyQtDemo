#include "editdialog.h"
#include <QLayout>
#include <QLabel>
#include <QSettings>

int EditDialog::widS = 64;
int EditDialog::heiS = 46;

EditDialog::EditDialog(QWidget *parent) : QDialog(parent)
{
    QLabel *wLabel = new QLabel(tr("宽 : "),this);
    QLabel *hLabel = new QLabel(tr("高 : "),this);
    widthEdit = new QLineEdit(this);
    heightEdit = new QLineEdit(this);


    QSettings setting("Gxin Studio","maze",this);
    int mw = setting.value("EditDialog/MazeWidth").toInt();
    widS = mw > 0 ? mw : widS;
    int mh = setting.value("EditDialog/MazeHeight").toInt();
    heiS = mh > 0 ? mh : heiS;

    widthEdit->setText(QString::number(widS));
    heightEdit->setText(QString::number(heiS));
    QHBoxLayout *aLayout = new QHBoxLayout;
    aLayout->addWidget(wLabel);
    aLayout->addWidget(widthEdit);
    QHBoxLayout *bLayout = new QHBoxLayout;
    bLayout->addWidget(hLabel);
    bLayout->addWidget(heightEdit);

    okButton = new QPushButton(tr("确定"),this);
    cancelButton = new QPushButton(tr("取消"),this);
    QHBoxLayout *cLayout = new QHBoxLayout;
    cLayout->addSpacing(5);
    cLayout->addWidget(okButton);
    cLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(aLayout);
    mainLayout->addLayout(bLayout);
    mainLayout->addLayout(cLayout);

    this->setLayout(mainLayout);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);

    connect(widthEdit,SIGNAL(textChanged(QString)),this,SLOT(regEdit()));
    connect(heightEdit,SIGNAL(textChanged(QString)),this,SLOT(regEdit()));
    connect(okButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

EditDialog::~EditDialog()
{

}

QPoint EditDialog::getData()
{
    widS = widthEdit->text().toInt();
    heiS = heightEdit->text().toInt();
    QSettings setting("Gxin Studio","maze",this);
    setting.setValue("EditDialog/MazeWidth", widS);
    setting.setValue("EditDialog/MazeHeight", heiS);
    return QPoint(widS,heiS);
}

void EditDialog::regEdit()
{
    QLineEdit *edit = qobject_cast<QLineEdit*>(this->sender());
    QString str = edit->text();
    int i;
    for(i = 0;i<str.length();i++){
        if(str[i]<'0' || str[i]>'9')
            break;
    }
    str = str.left(i);
    if(str.toInt()>=500)
        str = "498";

    edit->setText(str);
}

