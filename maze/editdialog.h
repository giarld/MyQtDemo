#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class EditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditDialog(QWidget *parent = 0);
    ~EditDialog();

    QPoint getData();

private slots:
    void regEdit();
private:
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;

    static int widS;
    static int heiS;
};

#endif // EDITDIALOG_H
