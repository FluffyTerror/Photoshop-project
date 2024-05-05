#ifndef CHANGECOLORPALETTE_H
#define CHANGECOLORPALETTE_H

#include <QWidget>

namespace Ui {
class changeColorPalette;
}

class changeColorPalette : public QWidget
{
    Q_OBJECT

public:
    explicit changeColorPalette(QWidget *parent = nullptr);
    ~changeColorPalette();

private slots:

    void on_ok_button_clicked();

    void on_close_button_clicked();

signals:
    void change_color(int);

private:
    Ui::changeColorPalette *ui;
};

#endif // CHANGECOLORPALETTE_H
