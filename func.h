#ifndef FUNC_H
#define FUNC_H
#include <QPixmap>
#include "medianfilter.h"
#include "msrcr.h"
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>

////////////////////Function///////////////////////
/// brightness \ mediafilter
///

namespace Funcs {
    QImage Brightness(int delta,QImage origin);
    QImage MeidaFilter(const QImage &origin, int radius);
    QImage Retinex(const QImage &origin);
    Mat qim2mat(QImage & qim);
}

#endif // FUNC_H
