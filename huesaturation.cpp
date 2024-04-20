#include "huesaturation.h"
#include "ui_huesaturation.h"

huesaturation::huesaturation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::huesaturation)
{
    ui->setupUi(this);
    ui->HueSlider->setRange(0, 360);
    ui->SaturationSlider->setRange(-100,100);
}

huesaturation::~huesaturation()
{
    delete ui;
}


void huesaturation::on_SaturationSlider_valueChanged(int value)
{
    ui->label_3->setText(QString::number(value));
}


void huesaturation::on_HueSlider_valueChanged(int value)
{
    ui->label_4->setText(QString::number(value));
}


void huesaturation::on_CancelButton_clicked()
{
     this->close();
}

