#include "huesaturation.h"
#include "ui_huesaturation.h"

huesaturation::huesaturation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::huesaturation)
{
    ui->setupUi(this);
}

huesaturation::~huesaturation()
{
    delete ui;
}

void huesaturation::on_SaturationSlider_valueChanged(int value)
{
    ui->label_3->setText(QString::number(value));
}


void huesaturation::on_HuelSlider_valueChanged(int value)
{
    ui->label_4->setText(QString::number(value));
}


void huesaturation::on_CancelButton_clicked()
{
     this->close();
}

