#include "cut_image_mod.h"
#include "ui_cut_image_mod.h"
#include "type_function.h"

Cut_image_mod::Cut_image_mod(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cut_image_mod)
{

    ui->setupUi(this);
    this->setWindowTitle("Cut");
    ui->XSlider->setRange(0,100);
    ui->YSlider->setRange(-100,0);
    ui->X2Slider->setRange(-100,0);
    ui->Y2Slider->setRange(0,100);
}

Cut_image_mod::~Cut_image_mod()
{
    delete ui;
}

void Cut_image_mod::on_XSlider_sliderMoved(int position)
{
    change_slider_position(x_fun_left, position);
}


void Cut_image_mod::on_X2Slider_sliderMoved(int position)
{
    change_slider_position(x_fun_right, position);
}


void Cut_image_mod::on_YSlider_sliderMoved(int position)
{
    change_slider_position(y_fun_up, position);
}


void Cut_image_mod::on_Y2Slider_sliderMoved(int position)
{
    change_slider_position(y_fun_down, position);
}


void Cut_image_mod::on_cut_ok_clicked()
{
    x = ui->XSlider->value();
    x2 = ui->X2Slider->value();
    y = ui->YSlider->value();
    y2 = ui->Y2Slider->value();
    cut_image_ok_press();
    this->close();
}


void Cut_image_mod::on_cut_cancellation_clicked()
{
    ui->XSlider->setValue(x);
    ui->X2Slider->setValue(x2);
    ui->YSlider->setValue(y);
    ui->Y2Slider->setValue(y2);
    cut_image_close_press();
    this->close();
}

void Cut_image_mod::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) {
        on_cut_cancellation_clicked(); // Вызов соответствующего метода при закрытии окна из-за нажатия на крестик
    }
    event->accept(); // Принимаем событие закрытия окна
}
