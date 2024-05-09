#include "huesaturation.h"
#include "qevent.h"
#include "ui_huesaturation.h"

huesaturation::huesaturation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::huesaturation)
{
// Настройка интерфейса и виджетов
ui->setupUi(this);
this->setWindowTitle("Monochrome");
ui->HueSlider->setRange(0, 359); // Установка диапазона для слайдера оттенка
ui->SaturationSlider->setRange(-100, 100); // Установка диапазона для слайдера насыщенности
ui->ValueSlider->setRange(-100, 100); // Установка диапазона для слайдера яркости

// Установка иконки приложения
QIcon icon(":/icons/mainicon.png");
this->setWindowIcon(icon);

// Сохранение начальных значений насыщенности и оттенка
initialSaturation = ui->SaturationSlider->value();
initialHue = ui->HueSlider->value();

// Создание градиента для слайдера оттенка
QString gradientColors;
for (int i = 0; i < 360; ++i) {
    QString color = QColor::fromHsv(i, 255, 255).name();
    gradientColors += QString("stop:%1 %2,").arg(QString::number(i / 360.0)).arg(color);
}

// Настройка стилей слайдера
QString styleSheet = QString("QSlider::groove:horizontal {"
                             "    border: 1px solid #999999;"
                             "    height: 10px;"
                             "    margin: 2px 0;"
                             "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, %1);"
                             "}"
                             "QSlider::handle:horizontal {"
                             "    background-color: #ffffff;"
                             "    border: 1px solid #5c5c5c;"
                             "    width: 30px;"
                             "    margin: -2px 0;"
                             "    border-radius: 3px;"
                             "}").arg(gradientColors)
                            + QString("QSlider::handle:horizontal {"
                             "background: white; /* Цвет фона ползунка */"
                             "border: 2px solid gray; /* Цвет рамки ползунка */"
                             "width: 18px; /* Ширина ползунка */"
                                 "margin-top: -2px; /* Отступ сверху */"
                                 "margin-bottom: -2px; /* Отступ снизу */"
                                 "border-radius: 9px; /* Радиус скругления углов */"
                         "}");


    ui->HueSlider->setStyleSheet(styleSheet);


}

huesaturation::~huesaturation()
{
    delete ui;
}



// Обработчик нажатия кнопки "Отмена"
void huesaturation::on_CancelButton_clicked()
{
    // Восстанавливаем значения насыщенности и оттенка
    ui->SaturationSlider->setValue(initialSaturation);
    ui->HueSlider->setValue(initialHue);

    // Генерируем сигнал об отмене операции и закрываем окно
    emit CancelMono();
    this->close();
}

// Обработчик нажатия кнопки "Принять"
void huesaturation::on_AcceptButton_clicked()
{
    // Передаем текущие значения насыщенности и оттенка через сигнал и закрываем окно
    emit parametersAccepted(ui->HueSlider->value(), ui->SaturationSlider->value(), ui->ValueSlider->value());
    this->close();
}

// Обработчик нажатия кнопки "Авто"
void huesaturation::on_AutoButton_clicked()
{
    // Генерируем сигнал об автоматическом принятии изменений и закрываем окно
    emit autoAccepted();
    this->close();
}

// Обработчик перемещения слайдера оттенка
void huesaturation::on_HueSlider_sliderMoved(int position)
{
    // Обновляем метку с текущим значением оттенка и передаем новые параметры через сигнал
    ui->label_4->setText(QString::number(position));
    emit parametersChanged(position, ui->SaturationSlider->value(), ui->ValueSlider->value());
}

// Обработчик перемещения слайдера насыщенности
void huesaturation::on_SaturationSlider_sliderMoved(int position)
{
    // Обновляем метку с текущим значением насыщенности и передаем новые параметры через сигнал
    ui->label_3->setText(QString::number(position));
    emit parametersChanged(ui->HueSlider->value(), position, ui->ValueSlider->value());
}

// Обработчик перемещения слайдера яркости
void huesaturation::on_ValueSlider_sliderMoved(int position)
{
    // Обновляем метку с текущим значением яркости и передаем новые параметры через сигнал
    ui->label_9->setText(QString::number(position));
    emit parametersChanged(ui->HueSlider->value(), ui->SaturationSlider->value(), position);
}

// Обработчик события закрытия окна
void huesaturation::closeEvent(QCloseEvent *event)
{
    // Если закрытие окна произошло по нажатию на кнопку "X"
    if (event->spontaneous()) {
        // Вызываем соответствующий метод для отмены изменений
        on_CancelButton_clicked();
    }
    // Принимаем событие закрытия окна
    event->accept();
}


