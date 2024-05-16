
#include "changecolorpalette.h"
#include "ui_changecolorpalette.h"

/**
 * @brief Конструктор формы для изменения цвета
 * @param parent Указатель на родительский виджет. По умолчанию равен nullptr.
 */
changeColorPalette::changeColorPalette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changeColorPalette)
{
    ui->setupUi(this);

    // Установка иконки приложения
    QIcon icon(":/icons/mainicon.png");
    this->setWindowIcon(icon);

    // Установка заголовка окна
    this->setWindowTitle("ChangeColor");

    // Установка диапазона для слайдера цвета
    ui->colorSlide->setRange(0, 360);

    // Создание градиента для слайдера цвета
    QString gradientColors;
    for (int i = 0; i < 360; ++i) {
        QString color = QColor::fromHsv(i, 255, 255).name();
        gradientColors += QString("stop:%1 %2,").arg(QString::number(i / 360.0)).arg(color);
    }

    // Настройка стилей слайдера цвета
    QString styleSheet = QString("QSlider::groove:horizontal {"
                                 "    border: 1px solid #999999;"
                                 "    height: 10px;"
                                 "    margin: 2px 0;"
                                 "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, %1);"
                                 "}"
                                 "QSlider::handle:horizontal {"
                                 "    background-color: #ffffff;"
                                 "    border: 1px solid #5c5c5c;"
                                 "    width: 18px;"
                                 "    margin: -2px 0;"
                                 "    border-radius: 3px;"
                                 "}").arg(gradientColors) + QString("QSlider::handle:horizontal {"
                                   "background: white; /* Цвет фона ползунка */"
                                   "border: 2px solid gray; /* Цвет рамки ползунка */"
                                   "width: 18px; /* Ширина ползунка */"
                                   "margin-top: -2px; /* Отступ сверху */"
                                   "margin-bottom: -2px; /* Отступ снизу */"
                                   "border-radius: 9px; /* Радиус скругления углов */"
                                   "}");

    // Установка стилей для слайдера цвета
    ui->colorSlide->setStyleSheet(styleSheet);
}

/**
 * @brief Деструктор класса changeColorPalette
 */
changeColorPalette::~changeColorPalette()
{
    delete ui;
}

/**
 * @brief Обработчик нажатия кнопки "OK"
 */
void changeColorPalette::on_ok_button_clicked()
{
    // Генерируем сигнал об изменении цвета и передаем новое значение цвета, затем закрываем окно
    emit change_color(ui->colorSlide->value());
    this->close();
}

/**
 * @brief Обработчик нажатия кнопки "Закрыть"
 */
void changeColorPalette::on_close_button_clicked()
{
    // Закрываем окно
    this->close();
}

/**
 * @brief Обработчик перемещения слайдера цвета
 * @param position Позиция слайдера
 */
void changeColorPalette::on_colorSlide_sliderMoved(int position)
{
    // Генерируем сигнал об изменении цвета и передаем новое значение цвета
    emit change_color(position);
}
