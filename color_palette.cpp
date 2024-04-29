#include "color_palette.h"
#include "ui_color_palette.h"
#include <QApplication>

color_palette::color_palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::color_palette)
{
    ui->setupUi(this);
    this->setWindowTitle("Palette");
}

color_palette::~color_palette()
{
    delete ui;
}

void color_palette::auto_color_pal(std::vector<QColor> colors)
{
    std::vector<QString> colorsString;
    ui->spinBox->setMaximum((colors.size() <= 20)? colors.size() : 20);
    ui->spinBox->setValue(ui->spinBox->maximum());
    for (int i = 0; i < colors.size() && i < 20; i++)
    {
        QString colorsString = colors[i].name();

        QLabel *label = new QLabel(this);
        labels.push_back(label);

        label->setStyleSheet("background-color: " + colorsString + "; border: 2px solid black");

        ui->layout_2->layout()->addWidget(label);
    }

}

void color_palette::on_spinBox_valueChanged(int arg1)
{
    for (QLabel* label : labels)
    {
        label->hide();
    }
    for (int i = 0; i < arg1 && i < labels.size(); i++)
    {
        labels[i]->show();
    }


}

void color_palette::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) {
        if(!labels.empty())
        {
            for (QLabel* label : labels)
            {
                delete label;
            }
            labels.clear();
        }
    }
    event->accept(); // Принимаем событие закрытия окна
}
