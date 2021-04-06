#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include "graphicsview.h"
#include "histogram.h"
#include "medianfilter.h"
#define WINDOW_CRITICAL "ERROR"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    //重写paintEvnet事件 画背景图
        void paintEvent(QPaintEvent *);
        void updateRightImage(QPixmap &pixmap);


        void createToolBar();

private slots:


        void on_actionAdd_triggered();//添加

        void on_actionDelete_triggered();//删除

        void on_actionSave_triggered();//保存

        void on_actionUndo_triggered();

        void on_actionHistogram_triggered();

        void on_actionBrightness_triggered();

        void on_actionMedia_Filter_triggered();

        void on_actionRetinex_triggered();

private:
    Ui::MainWindow *ui;

    QGraphicsScene  *leftScene;
    QGraphicsScene  *rightScene;
    QGraphicsPixmapItem* leftPixmapItem;
    QGraphicsPixmapItem* rightPixmapItem;

};
#endif // MAINWINDOW_H
