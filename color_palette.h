#ifndef COLOR_PALETTE_H
#define COLOR_PALETTE_H

#include <QWidget>
#include <QString>
#include <vector>
#include <QCloseEvent>
#include <QLabel>
#include <QPushButton>
#include "changecolorpalette.h"

/**
 * @brief Форма цветовой палитры
 */
namespace Ui {
class color_palette;
}

/**
 * @brief Класс цветовой палитры
 */
class color_palette : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса color_palette
     * @param parent Указатель на родительский виджет. По умолчанию равен nullptr.
     */
    explicit color_palette(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса color_palette
     */
    ~color_palette();

public slots:
    /**
     * @brief Слот для функции автоматического создания цветовой палитры
     * @param colors Вектор цветов
     */
    void auto_color_pal(std::vector<QColor> colors);

    /**
     * @brief Слот для обработчика изменения цвета
     * @param pos Позиция ползунка
     */
    void change_color(int pos);

    /**
     * @brief Слот для обновления цвета в палитре
     * @param NewColor Новый цвет
     */
    void slots_color_pallete_allow_adding_a_new_color(QColor NewColor);

private slots:
    /**
     * @brief Слот для изменения значения spinBox
     * @param arg1 Новое значение
     */
    void on_spinBox_valueChanged(int arg1);

    /**
     * @brief Обработчик события закрытия окна
     * @param event Указатель на событие закрытия окна
     */
    void closeEvent(QCloseEvent *event);

    /**
     * @brief Обработчик нажатия на кнопку с цветом
     * @param num Номер кнопки
     */
    void handleColorButtonClicked(int num);

    /**
     * @brief Обработчик нажатия кнопки "OK"
     */
    void on_ok_button_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Закрыть"
     */
    void on_close_button_clicked();

private:
    /**
     * @brief Указатель на форму палитры
     */
    Ui::color_palette *ui;

    /**
     * @brief Вектор для хранения кнопок
     */
    std::vector<QPushButton*> buttons;

    /**
     * @brief Объект класса для изменения палитры
     */
    changeColorPalette *ChangeColor_Main;

    /**
     * @brief Вектор для хранения цветов
     */
    std::vector<QColor> QColorsPalette;

    /**
     * @brief Функция очистки палитры
     */
    void clearPalette();

signals:
    /**
     * @brief Сигнал для изменения изображения палитры
     * @param NewColor Новый цвет
     */
    void changeColorPaletteImage(QColor NewColor);

    /**
     * @brief Сигнал нажатия кнопки "OK"
     */
    void paletteOkClick();

    /**
     * @brief Сигнал для нажатия кнопки "Закрыть"
     */
    void paletteCloseClick();

    /**
     * @brief Сигнал для обновления цвета в палитре
     * @param OldColor Старый цвет
     */
    void color_button_clicked(QColor OldColor);
};

#endif // COLOR_PALETTE_H
