#ifndef CUT_IMAGE_MOD_H
#define CUT_IMAGE_MOD_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class Cut_image_mod;
}

/**
 * @brief Класс Cut_image_mod
 * Класс для модуля обрезки изображения
 */
class Cut_image_mod : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор формы для модуля обрезки изображения
     * @param parent Указатель на родительский виджет. По умолчанию равен nullptr.
     */
    explicit Cut_image_mod(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса Cut_image_mod
     */
    ~Cut_image_mod();

private slots:
    /**
     * @brief Обработчик перемещения слайдера X
     * @param position Позиция слайдера
     */
    void on_XSlider_sliderMoved(int position);

    /**
     * @brief Обработчик перемещения слайдера X2
     * @param position Позиция слайдера
     */
    void on_X2Slider_sliderMoved(int position);

    /**
     * @brief Обработчик перемещения слайдера Y
     * @param position Позиция слайдера
     */
    void on_YSlider_sliderMoved(int position);

    /**
     * @brief Обработчик перемещения слайдера Y2
     * @param position Позиция слайдера
     */
    void on_Y2Slider_sliderMoved(int position);

    /**
     * @brief Обработчик нажатия кнопки "OK" для обрезки
     */
    void on_cut_ok_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Отмена" для обрезки
     */
    void on_cut_cancellation_clicked();

    /**
     * @brief Обработчик события закрытия окна
     * @param event Событие закрытия окна
     */
    void closeEvent(QCloseEvent *event);

private:
    // Указатель на UI форму
    Ui::Cut_image_mod *ui;

    // Позиции для обрезки изображения
    int x = 0, y = 0, x2 = 0, y2 = 0; ///< x - обрезает слева, x2 - справа, y - сверху, y2 - снизу

signals:
    /**
     * @brief Сигнал изменения позиции слайдера
     * @param num_type_cut Тип обрезки
     * @param num_cropped_pixel Количество обрезанных пикселей
     */
    void change_slider_position(short num_type_cut, int num_cropped_pixel);

    /**
     * @brief Сигнал нажатия кнопки "OK" для обрезки
     */
    void cut_image_ok_press();

    /**
     * @brief Сигнал нажатия кнопки "Закрыть"
     */
    void cut_image_close_press();
};

#endif // CUT_IMAGE_MOD_H
