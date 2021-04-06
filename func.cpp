#include "func.h"

////////////////////////////////////////////////////////////////////////////////////////
/////////////////////             Adjust image brightness           ////////////////////
////////////////////////////////////////////////////////////////////////////////////////
QImage Funcs::Brightness(int delta,QImage origin){
    QImage *newImage = new QImage(origin.width(), origin.height(),
                                  QImage::Format_ARGB32);

    QColor oldColor;
    int r, g, b;

    for (int x=0; x<newImage->width(); x++)
    {
        for (int y=0; y<newImage->height(); y++)
        {
            oldColor = QColor(origin.pixel(x,y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue() + delta;

            // Check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            newImage->setPixel(x,y, qRgb(r,g,b));
        }
    }
    return *newImage;
}






//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////                     Media Filter                       /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
QImage Funcs::MeidaFilter(const QImage &origin, int filterRadius)
{
    int imageHeight = origin.height();
    int imageWidth = origin.width();
    MedianFilter medianFilter;
    int* resImageBits = new int[imageHeight * imageWidth];
    medianFilter.applyMedianFilter((int*)origin.bits(), resImageBits, imageHeight, imageWidth, filterRadius);

    QImage destImage((uchar*)resImageBits, imageWidth, imageHeight, origin.format());
//    QPixmap pixRes;
//    pixRes.convertFromImage(destImage);


    return destImage;
}

Mat Funcs:: qim2mat(QImage & qim)
{
    Mat mat;
    switch(qim.format()){
        case QImage::Format_RGB888:
            mat = Mat(qim.height(), qim.width(),
                CV_8UC3,(void*)qim.constBits(),qim.bytesPerLine());
            break;
        case QImage::Format_ARGB32_Premultiplied:
            mat = Mat(qim.height(), qim.width(),
                CV_8UC4,(void*)qim.constBits(),qim.bytesPerLine());
            break;
    }
    return mat;
}



//////////////////////////////////////////////////////////////////////////////////////////
////////////////                        Retinex                          /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
 QImage Funcs::Retinex(const QImage &origin)
{

}
