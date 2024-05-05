#ifndef COLOR_PALETTE_H
#define COLOR_PALETTE_H

#include <QWidget>
#include <QString>
#include <vector>
#include<QCloseEvent>
#include <QLabel>
#include <QPushButton>
#include "changecolorpalette.h"
namespace Ui {
class color_palette;
}

class color_palette : public QWidget
{
    Q_OBJECT

public:
    explicit color_palette(QWidget *parent = nullptr);
    ~color_palette();
public slots:
    void auto_color_pal(std::vector<QColor> colors);
    void change_color(int pos);
    void slots_color_pallete_allow_adding_a_new_color(QColor);
private slots:
    void on_spinBox_valueChanged(int arg1);
    void closeEvent(QCloseEvent *event);
    void handleColorButtonClicked(int num);

    void on_ok_button_clicked();

    void on_close_button_clicked();

private:
    Ui::color_palette *ui;
    std::vector<QPushButton*> buttons;
    changeColorPalette *ChangeColor_Main;
    std::vector<QColor> QColorsPalette;
    void clearPalette();
signals:
    void changeColorPaletteImage(QColor NewColor, QColor OldColor);
    void paletteOkClick();
    void paletteCloseClick();
};

#endif // COLOR_PALETTE_H
