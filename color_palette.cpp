#include "color_palette.h"
#include "ui_color_palette.h"
#include <QApplication>
#include <QMessageBox>
color_palette::color_palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::color_palette)
{
    ui->setupUi(this);
    this->setWindowTitle("Palette");
}

int global_num_button;

color_palette::~color_palette()
{
    delete ui;
    delete ChangeColor_Main;
    for (int i = 0; i <= buttons.size(); i++)
    {
        delete buttons[i];
    }
    buttons.clear();
    QColorsPalette.clear();
}

void color_palette::auto_color_pal(std::vector<QColor> colors)
{
    std::vector<QString> colorsString;
    QColorsPalette = colors;
    ChangeColor_Main = new changeColorPalette();
    connect(ChangeColor_Main, &changeColorPalette::change_color, this, &color_palette::change_color);
    ui->spinBox->setMaximum((colors.size() <= 20) ? colors.size() : 20);
    ui->spinBox->setValue(ui->spinBox->maximum());

    for (int i = 0; i < colors.size() && i < 20; i++)
    {
        QString colorString = colors[i].name();

        QPushButton *button = new QPushButton(this);
        buttons.push_back(button);

        button->setStyleSheet("QPushButton {background-color: " + colorString + "; border: 2px solid black;}"
                              "QPushButton:hover { border: 3px solid red }");
        button->setMaximumWidth(1000);

        // Установка политики размеров для кнопки
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        ui->layout_2->layout()->addWidget(button);

        // Связывание сигнала clicked с нужным слотом
        connect(button, &QPushButton::clicked, this, [=]() {
            handleColorButtonClicked(i);
        });
    }
}

void color_palette::change_color(int pos)
{
        QString colorString;
        QColor newColor = QColor::fromHsv((pos * 359) / 360, 255, 255);
        colorString = newColor.name();
        newColor = QColor::fromHsv((pos * 359) / 360, 200, 200);
        buttons[global_num_button]->setStyleSheet(("QPushButton {background-color: " + colorString + "; border: 2px solid black;}"
                                                   "QPushButton:hover { border: 3px solid red }"));
        changeColorPaletteImage(newColor, QColorsPalette[global_num_button]);
}

void color_palette::slots_color_pallete_allow_adding_a_new_color(QColor NewColorPal)
{
        QColorsPalette[global_num_button] = NewColorPal;
}

void color_palette::handleColorButtonClicked(int num)
{
    global_num_button = num;
    ChangeColor_Main->show();
}

void color_palette::on_spinBox_valueChanged(int arg1)
{
    for (QPushButton* button : buttons)
    {
        button->hide();
    }
    for (int i = 0; i < arg1 && i < buttons.size(); i++)
    {
        buttons[i]->show();
    }


}

void color_palette::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) {
        on_close_button_clicked();
    }
    event->accept(); // Принимаем событие закрытия окна
}

void color_palette::on_ok_button_clicked()
{
    clearPalette();
    paletteOkClick();
}

void color_palette::clearPalette()
{
    for (int i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
    }
    buttons.clear();
    QColorsPalette.clear();
    this->close();
}


void color_palette::on_close_button_clicked()
{
    clearPalette();
    paletteCloseClick();
}

