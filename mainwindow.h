#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "events.h"
#include "huesaturation.h"
#include "qgraphicsitem.h"
#include <QMainWindow>
#include "cut_image_mod.h"
#include "color_palette.h"
#include <vector>

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
    QGraphicsPixmapItem * CutPixmapItem;
    QGraphicsPixmapItem * PalettePixmapItem;
    QGraphicsPixmapItem * originalPixmapItem;
    huesaturation *hueSaturationForm;
    QImage *loadedImage;
    QImage CopyColorImage, CopyColorImageT;
    Custom_View *customView;
    Cut_image_mod *cut_image;
    color_palette *color_pal;

public slots:
    void on_Select_clicked();
    void on_Monochrome_clicked();
    void on_Accept();
    void on_MonochromeAuto();
    void on_CancelMono();
    void ImageAccept(const QString &filepath);
    void on_Cut_clicked();
    void change_size_image(short type, int num_cropped_pixel);
    void cut_button_clicked();
    void close_cut_button_clicked();
    void on_color_pal_clicked();
    void on_SaveButton_clicked();
    void changeColorPallete(QColor NewColor);
    void paletteOkClick();
    void paletteCloseClick();
    void on_MonochromeParametersChanged(int hue,int saturation, int value);
    void clicked_color_button_in_pallete(QColor OldColor);
signals:
    void color_pallete_inf(std::vector<QColor> colors_pal);
    void color_pallete_allow_adding_a_new_color(QColor);

private:
    struct coord_color {
        int x;
        int y;
        QColor color;
    };
    coord_color make_coord_color(int, int, QColor);
    Ui::MainWindow *ui;
    void crop_image(QImage cut_image_t,QRect cropRect_t);
    void colors_sort();
    std::vector <coord_color> change_pixel_map_pallete;
};
#endif // MAINWINDOW_H
