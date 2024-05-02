#ifndef COLOR_PALETTE_H
#define COLOR_PALETTE_H

#include "change_colors.h"
#include <QWidget>
#include <QString>
#include <vector>
#include<QCloseEvent>
#include <QLabel>
#include <QPushButton>
namespace Ui {
class color_palette;
}

class color_palette : public QWidget
{
    Q_OBJECT

public:
    explicit color_palette(QWidget *parent = nullptr);
    ~color_palette();
    Change_colors *ChangeWindow;
public slots:
    void auto_color_pal(std::vector<QColor> colors);
private slots:
    void on_spinBox_valueChanged(int arg1);
    void closeEvent(QCloseEvent *event);
    void handleColorButtonClicked(const QColor &color);

private:
    Ui::color_palette *ui;
    std::vector<QPushButton*> buttons;
};

#endif // COLOR_PALETTE_H
