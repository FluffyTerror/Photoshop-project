#ifndef CHANGECOLORPALETTE_H
#define CHANGECOLORPALETTE_H

#include <QWidget>

namespace Ui {
class changeColorPalette;
}

/**
 * @brief Класс changeColorPalette
 * Класс для формы изменения цвета
 */
class changeColorPalette : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор формы для изменения цвета
     * @param parent Указатель на родительский виджет. По умолчанию равен nullptr.
     */
    explicit changeColorPalette(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса changeColorPalette
     */
    ~changeColorPalette();

private slots:
    /**
     * @brief Обработчик нажатия кнопки "OK"
     */
    void on_ok_button_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Закрыть"
     */
    void on_close_button_clicked();

    /**
     * @brief Обработчик перемещения слайдера цвета
     * @param position Позиция слайдера
     */
    void on_colorSlide_sliderMoved(int position);

signals:
    /**
     * @brief Сигнал изменения цвета
     * @param Новый цвет
     */
    void change_color(int);

private:
    // Указатель на UI форму
    Ui::changeColorPalette *ui;
};

#endif // CHANGECOLORPALETTE_H
