#include "makeblurimage.h"


MakeBlurImage::MakeBlurImage(QObject *parent) : QObject(parent)
{

}

void MakeBlurImage::setImage(const QString &filename)
{
    mImage.load(filename);
    //    emit makeOver(mImage);
}

void MakeBlurImage::makeBlur(int lever)
{
    QMutexLocker loker(&mMutex);

    if(lever == 0){
        emit makeOver(mImage);
        return;
    }
    int width = mImage.width();
    int height = mImage.height();
    for(int x = 0; x<width; x++){
        for(int y = 0; y<height; y++){
            QRgb pixs = 0;
            QColor pcolor(pixs);
            int red = pcolor.red();
            int green = pcolor.green();
            int blue = pcolor.blue();
            int ps = 0;
            for(int i = -lever; i <= lever; i++){
                for(int j = -lever; j <= lever; j++){
                    if(i==0 && j==0){
                        continue;
                    }
                    int xx = x+i;
                    int yy = y+j;
                    if(xx<0 || yy<0 || xx>=width || yy>=height) continue;
                    QColor temp(mImage.pixel(xx,yy));
                    red += temp.red();
                    blue += temp.blue();
                    green += temp.green();
                    ps++;
                }
            }
            red/=ps;
            blue/=ps;
            green/=ps;
            QColor zz(red,green,blue,pcolor.alpha());
            mImage.setPixel(x,y,zz.rgb());
        }
    }
    emit makeOver(mImage);
}
