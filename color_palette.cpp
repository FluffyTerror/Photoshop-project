

#include "color_palette.h"
#include "ui_color_palette.h"
#include <QApplication>
#include <QMessageBox>

/**
 * @brief Конструктор класса color_palette.
 *
 * @param parent Указатель на родительский виджет. По умолчанию равен nullptr.
 */
color_palette::color_palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::color_palette)
{
    ui->setupUi(this);
    this->setWindowTitle("Palette");
    QIcon icon(":/icons/mainicon.png");
    this->setWindowIcon(icon);
}

int global_num_button;

/**
 * @brief Деструктор класса color_palette.
 */
color_palette::~color_palette()
{
    // Освобождение памяти, выделенной для ui
    delete ui;

    // Освобождение памяти, выделенной для ChangeColor_Main
    delete ChangeColor_Main;

    // Очистка контейнера buttons и освобождение памяти, выделенной для кнопок
    for (int i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
    }
    buttons.clear();

    // Очистка контейнера QColorsPalette
    QColorsPalette.clear();
}

/**
 * @brief Функция автоматического создания цветовой палитры.
 *
 * @param colors Вектор цветов для создания палитры.
 */
void color_palette::auto_color_pal(std::vector<QColor> colors)
{
    // Инициализация контейнера colorsString
    std::vector<QString> colorsString;

    // Сохранение переданных цветов в QColorsPalette
    QColorsPalette = colors;

    // Создание экземпляра класса changeColorPalette и установка соединения сигнала change_color
    ChangeColor_Main = new changeColorPalette();
    connect(ChangeColor_Main, &changeColorPalette::change_color, this, &color_palette::change_color);

    // Установка максимального значения для spinBox
    ui->spinBox->setMaximum((colors.size() <= 20) ? colors.size() : 20);
    ui->spinBox->setValue(ui->spinBox->maximum());

    // Создание кнопок и добавление их в layout
    for (int i = 0; i < colors.size() && i < 20; i++)
    {
        QString colorString = colors[i].name();
        QPushButton *button = new QPushButton(this);
        buttons.push_back(button);
        button->setStyleSheet("QPushButton {background-color: " + colorString + "; border: 2px solid black;}"
                                                                                "QPushButton:hover { border: 3px solid red }");
        button->setMaximumWidth(1000);
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        ui->layout_2->layout()->addWidget(button);
        connect(button, &QPushButton::clicked, this, [=]() {
            handleColorButtonClicked(i);
        });
    }
}

/**
 * @brief Обработчик изменения цвета.
 *
 * @param pos Позиция нового цвета.
 */
void color_palette::change_color(int pos)
{
    QString colorString;
    QColor newColor = QColor::fromHsv((pos * 359) / 360, 255, 255);
    colorString = newColor.name();
    newColor = QColor::fromHsv((pos * 359) / 360, 200, 200);
    buttons[global_num_button]->setStyleSheet(("QPushButton {background-color: " + colorString + "; border: 2px solid black;}"
                                                                                                 "QPushButton:hover { border: 3px solid red }"));
    emit changeColorPaletteImage(newColor);
}

/**
 * @brief Слот для обновления цвета в палитре.
 *
 * @param NewColorPal Новый цвет для палитры.
 */
void color_palette::slots_color_pallete_allow_adding_a_new_color(QColor NewColorPal)
{
    QColorsPalette[global_num_button] = NewColorPal;
}

/**
 * @brief Обработчик нажатия на кнопку с цветом.
 *
 * @param num Номер кнопки.
 */
void color_palette::handleColorButtonClicked(int num)
{
    global_num_button = num;
    ChangeColor_Main->show();
    emit color_button_clicked(QColorsPalette[global_num_button]);
}

/**
 * @brief Обработчик изменения значения spinBox.
 *
 * @param arg1 Новое значение spinBox.
 */
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

/**
 * @brief Обработчик события закрытия окна.
 *
 * @param event Указатель на событие закрытия окна.
 */
void color_palette::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) {
        on_close_button_clicked();
    }
    event->accept(); // Принимаем событие закрытия окна
}

/**
 * @brief Обработчик нажатия кнопки "OK".
 */
void color_palette::on_ok_button_clicked()
{
    // Очищаем палитру и генерируем сигнал о нажатии кнопки "OK"
    clearPalette();
    paletteOkClick();
}

/**
 * @brief Функция очистки палитры.
 */
void color_palette::clearPalette()
{
    // Очищаем контейнер buttons и освобождаем память, выделенную для кнопок
    for (int i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
    }
    buttons.clear();

    // Очищаем контейнер QColorsPalette
    QColorsPalette.clear();

    // Закрываем окно
    this->close();
}

/**
 * @brief Обработчик нажатия кнопки "Закрыть".
 */
void color_palette::on_close_button_clicked()
{
    // Очищаем палитру и генерируем сигнал о нажатии кнопки "Закрыть"
    clearPalette();
    paletteCloseClick();
}
