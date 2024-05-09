#include "cut_image_mod.h"
#include "ui_cut_image_mod.h"
#include "type_function.h"

// Конструктор класса Cut_image_mod
Cut_image_mod::Cut_image_mod(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cut_image_mod)
{
    // Инициализация пользовательского интерфейса
    ui->setupUi(this);

    // Установка заголовка окна
    this->setWindowTitle("Cut");

    // Установка диапазонов для слайдеров
    ui->XSlider->setRange(0, 100);
    ui->YSlider->setRange(-100, 0);
    ui->X2Slider->setRange(-100, 0);
    ui->Y2Slider->setRange(0, 100);

    // Установка иконки приложения
    QIcon icon(":/icons/mainicon.png");
    this->setWindowIcon(icon);
}

// Деструктор класса Cut_image_mod
Cut_image_mod::~Cut_image_mod()
{
    // Освобождение памяти, выделенной для ui
    delete ui;
}

// Обработчик перемещения слайдера X
void Cut_image_mod::on_XSlider_sliderMoved(int position)
{
    // Генерируем сигнал о перемещении слайдера X и передаем новую позицию
    emit change_slider_position(x_fun_left, position);
}

// Обработчик перемещения слайдера X2
void Cut_image_mod::on_X2Slider_sliderMoved(int position)
{
    // Генерируем сигнал о перемещении слайдера X2 и передаем новую позицию
    emit change_slider_position(x_fun_right, position);
}

// Обработчик перемещения слайдера Y
void Cut_image_mod::on_YSlider_sliderMoved(int position)
{
    // Генерируем сигнал о перемещении слайдера Y и передаем новую позицию
    emit change_slider_position(y_fun_up, position);
}

// Обработчик перемещения слайдера Y2
void Cut_image_mod::on_Y2Slider_sliderMoved(int position)
{
    // Генерируем сигнал о перемещении слайдера Y2 и передаем новую позицию
    emit change_slider_position(y_fun_down, position);
}

// Обработчик нажатия кнопки "OK" для обрезки изображения
void Cut_image_mod::on_cut_ok_clicked()
{
    // Сохраняем текущие значения позиций слайдеров
    x = ui->XSlider->value();
    x2 = ui->X2Slider->value();
    y = ui->YSlider->value();
    y2 = ui->Y2Slider->value();

    // Генерируем сигнал об успешном завершении обрезки изображения и закрываем окно
    emit cut_image_ok_press();
    this->close();
}

// Обработчик нажатия кнопки "Отмена" для отмены обрезки изображения
void Cut_image_mod::on_cut_cancellation_clicked()
{
    // Восстанавливаем предыдущие значения позиций слайдеров
    ui->XSlider->setValue(x);
    ui->X2Slider->setValue(x2);
    ui->YSlider->setValue(y);
    ui->Y2Slider->setValue(y2);

    // Генерируем сигнал об отмене обрезки изображения и закрываем окно
    emit cut_image_close_press();
    this->close();
}

// Обработчик события закрытия окна
void Cut_image_mod::closeEvent(QCloseEvent *event)
{
    // Если закрытие окна произошло по нажатию на кнопку "X"
    if (event->spontaneous()) {
        // Вызываем соответствующий метод для отмены изменений
        on_cut_cancellation_clicked();
    }
    // Принимаем событие закрытия окна
    event->accept();
}



