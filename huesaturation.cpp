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
