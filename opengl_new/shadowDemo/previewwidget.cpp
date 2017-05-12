#include "previewwidget.h"
#include "QPainter"

PreviewWidget::PreviewWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(tr("预览"));
}

void PreviewWidget::setPreviewImage(const QImage &img)
{
    mPreviewImage = img;
    update();
}

void PreviewWidget::paintEvent(QPaintEvent *event)
{
    if(mPreviewImage.isNull())
        return;
    QPainter painter(this);

    painter.drawImage(QRect(0, 0, this->width(), this->height()), mPreviewImage);
}
