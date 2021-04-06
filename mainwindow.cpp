#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "func.h"
#include <QGraphicsPixmapItem>
#include <QToolBar>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ////////////////////////设置界面////////////////
    setWindowIcon(QIcon(":/res/mainp.png"));

    //set tiltle
    setWindowTitle("IProcess");

    leftScene = new QGraphicsScene;
    rightScene = new QGraphicsScene;

    leftPixmapItem = new QGraphicsPixmapItem();
    rightPixmapItem = new QGraphicsPixmapItem();

    //设置QGraphicsView透明度问题！！！  不能在 sestbackgroundbrush中直接设置
    //透明度在QWidget的stylesheet中设置

    leftScene->setBackgroundBrush(QColor::fromRgb(224,224,224));
    ui->leftGraphicsView->setScene(leftScene);
    rightScene->setBackgroundBrush(QColor::fromRgb(224,224,224));
    ui->rightGraphicsView->setScene(rightScene);


    createToolBar();
}




//////////////////////////////////////////////////////////////////////////////////////////
////////////////                          设置工具栏                       /////////////////
/// //////////////////////////////////////////////////////////////////////////////////////
void MainWindow::createToolBar()
{
    QToolBar *mybar = new QToolBar(this);

    //将工具栏放入窗口中，参数一：默认停靠区域，参数二：toolbar对象
    addToolBar(Qt::LeftToolBarArea,mybar);

    //设置只允许左右停靠
    //mybar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);

    //设置浮动
    //mybar->setFloatable(false);

    //设置移动（关闭后无法拖动，总开关）
    //mybar->setMovable(false);
    mybar->addAction(ui->actionHistogram);


}




/////////////////////////////////////////设置背景/////////////////////////////////////////////
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/waterbgp.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}




MainWindow::~MainWindow()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////
////////////////                          打开文件                        /////////////////
/// //////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionAdd_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this,tr("Open Image"),"C:\\Users\\user",tr("All Files (*);;"
                                                                                               "All Images (*.bpm *.gif *.jpg *.jpeg *.png *.ppm *.xbm *.xpm);;"
                                                                                               "Image BPM (*.bpm);;"
                                                                                               "Image GIF (*.gif);;"
                                                                                               "Image JPG (*.jpg);;"
                                                                                               "Image JPEG (*.jpeg);;"
                                                                                               "Image PNG (*.png);;"
                                                                                               "Image PPM (*.ppm);;"
                                                                                               "Image XBM (*.xbm);;"
                                                                                               "Image XPM (*.xpm);;"));
    if(!imagePath.isEmpty())
    {
        QFile file(imagePath);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this,tr(WINDOW_CRITICAL),tr("Faile to open!"));
            return;
        }

        //delete previous one
        on_actionDelete_triggered();

        //uplaod image
        QPixmap leftpix(imagePath);

        leftPixmapItem = leftScene->addPixmap(leftpix);
        leftScene->setSceneRect(QRectF(leftpix.rect()));

        rightPixmapItem = rightScene->addPixmap(leftpix);
        rightScene->setSceneRect(QRect(leftpix.rect()));


    }
}




//////////////////////////////////////////////////////////////////////////////////////////
////////////////                         删除文件                         /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionDelete_triggered()
{
    leftScene->clear();
    ui->leftGraphicsView->resetTransform();

    rightScene->clear();
    ui->rightGraphicsView->resetTransform();

}




//////////////////////////////////////////////////////////////////////////////////////////
////////////////                         保存文件                         /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionSave_triggered()
{
    QString newPath = QFileDialog::getSaveFileName(this,tr("Save Image"),QString(),
                                                   tr("All files (*);;"
                                                      "Image BPM (*.bpm);;"
                                                      "Image GIF (*.gif);;"
                                                      "Image JPG (*.jpg);;"
                                                      "Image JPEG (*.jpeg);;"
                                                      "Image PNG (*.png);;"
                                                      "Image PPM (*.ppm);;"
                                                      "Image XBM (*.xbm);;"
                                                      "Image XPM (*.xpm);;"));
    if(!newPath.isEmpty()){
        QFile file(newPath);
        if(!file.open(QIODevice::WriteOnly)){
                QMessageBox::critical(this,"Error",tr("Faile to save"));
                return;
        }
    }
}




//////////////////////////////////////////////////////////////////////////////////////////
////////////////                          恢复                            /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionUndo_triggered()
{
    QPixmap leftImage = leftPixmapItem->pixmap();
   updateRightImage(leftImage);
   ui->rightGraphicsView->resetTransform();
}




//////////////////////////////////////////////////////////////////////////////////////////
////////////////                          update                         /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::updateRightImage(QPixmap &pixmap)
{
    rightPixmapItem->setPixmap(pixmap);
    rightScene->setSceneRect(QRectF(pixmap.rect()));
}




//////////////////////////////////////////////////////////////////////////////////////////
////////////////                         绘制图像直方图                    /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionHistogram_triggered()
    {

        QDialog * hstgrmDialog = new QDialog(this);
        QScrollArea * scrollArea = new QScrollArea(hstgrmDialog);

        Histogram * hstgrm = new Histogram(scrollArea);
        hstgrm->computeHstgrm(rightPixmapItem->pixmap().toImage());

        if (hstgrm == nullptr)
            return;


        scrollArea->setWidget(hstgrm);

        QHBoxLayout * layout = new QHBoxLayout;
        layout->addWidget(scrollArea);
        hstgrmDialog->setLayout(layout);

        hstgrm->resize(800, 780);
        hstgrmDialog->setFixedWidth(820);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->adjustSize();

        hstgrmDialog->setWindowTitle("Histogram - IProces");

        hstgrmDialog->show();
    }




//////////////////////////////////////////////////////////////////////////////////////////
////////////////                         调整亮度                         /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionBrightness_triggered()
{
    bool ok;
    int delta = QInputDialog::getInt(this,
                                     tr("Brightness"),
                                     "Input a value for brightness(+/-)",
                                     0,-1000,1000,10,&ok);
    if (ok)
    {
        if (delta != 100)
        {
            QPixmap rightImage = rightPixmapItem->pixmap();

            QImage newImage = Funcs::Brightness(delta, rightImage.toImage());
            rightImage.convertFromImage(newImage);

            updateRightImage(rightImage);
        }
        else
        {
            return;
        }
    }

}





//////////////////////////////////////////////////////////////////////////////////////////
////////////////                        中值滤波                          /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionMedia_Filter_triggered()
{
    bool ok;
    int value = QInputDialog::getInt(this, tr("Media Filter"), "Input a value for radius(1~30)",3,1,30,1,&ok);
    if (ok)
    {
        QPixmap rightImage = rightPixmapItem->pixmap();
        QImage newImage = Funcs::MeidaFilter(rightImage.toImage(), value);
        rightImage.convertFromImage(newImage);

        updateRightImage(rightImage);
    }

}



//////////////////////////////////////////////////////////////////////////////////////////
////////////////                        Retinex                          /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionRetinex_triggered()
{/*
    QPixmap rightImage = rightPixmapItem->pixmap();
    QImage newImage = Funcs::Retinex(rightImage.toImage());
    updateRightImage(rightImage);*/
}
