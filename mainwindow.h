#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qgraphicsitem.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsPixmapItem * monochromePixmapItem;
    QGraphicsPixmapItem * originalPixmapItem;


public slots:
   void on_Select_clicked();
   void on_Monochrome_clicked();
   void on_valuesChangedSaturation(int saturation);
   void on_valuesChangedHue(int hue);
   void on_MonochromeParametersChanged(int saturation, int hue);
   private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
