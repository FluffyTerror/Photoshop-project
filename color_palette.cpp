#include "color_palette.h"
#include "change_colors.h"
#include "ui_color_palette.h"
#include <QApplication>
#include <QMessageBox>
color_palette::color_palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::color_palette)
{
    ui->setupUi(this);
    this->setWindowTitle("Palette");
    ChangeWindow = new Change_colors();

}

color_palette::~color_palette()
{
    delete ui;
}

void color_palette::auto_color_pal(std::vector<QColor> colors)
{
    std::vector<QString> colorsString;
    ui->spinBox->setMaximum((colors.size() <= 20) ? colors.size() : 20);
    ui->spinBox->setValue(ui->spinBox->maximum());

    for (int i = 0; i < colors.size() && i < 20; i++)
    {
        QString colorString = colors[i].name();

        QPushButton *button = new QPushButton(this);
        buttons.push_back(button);

        button->setStyleSheet("background-color: " + colorString + "; border: 2px solid black");

        // Установка политики размеров для кнопки
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        ui->layout_2->layout()->addWidget(button);

        // Связывание сигнала clicked с нужным слотом
        connect(button, &QPushButton::clicked, this, [=]() {
            handleColorButtonClicked(colors[i]);
        });
    }
}

void color_palette::handleColorButtonClicked(const QColor &color)
{
    ChangeWindow->show();
    //ну тут мы получиили цвет кнопки а что дальше то делать я не знаю

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
        if(!buttons.empty())
        {
            for (QPushButton* button : buttons)
            {
                delete button;
            }
            buttons.clear();
        }
    }
    event->accept(); // Принимаем событие закрытия окна
}
