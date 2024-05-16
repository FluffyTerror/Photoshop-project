#ifndef HUESATURATION_H
#define HUESATURATION_H

#include <QWidget>

namespace Ui {
class huesaturation;
}

/**
 * @brief Класс huesaturation предоставляет интерфейс для изменения оттенка, насыщенности,яркости и преобразования изображения в монохромное.
 */
class huesaturation : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса huesaturation
     * @param parent Указатель на родительский виджет
     */
    explicit huesaturation(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса huesaturation
     */
    ~huesaturation();

private slots:
    /**
     * @brief Обработчик нажатия кнопки "Авто"
     */
    void on_AutoButton_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Отмена"
     */
    void on_CancelButton_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Принять"
     */
    void on_AcceptButton_clicked();

    /**
     * @brief Обработчик перемещения слайдера оттенка
     * @param position Текущая позиция слайдера
     */
    void on_HueSlider_sliderMoved(int position);

    /**
     * @brief Обработчик перемещения слайдера насыщенности
     * @param position Текущая позиция слайдера
     */
    void on_SaturationSlider_sliderMoved(int position);

    /**
     * @brief Обработчик перемещения слайдера яркости
     * @param position Текущая позиция слайдера
     */
    void on_ValueSlider_sliderMoved(int position);

    /**
     * @brief Обработчик события закрытия окна
     * @param event Указатель на событие закрытия окна
     */
    void closeEvent(QCloseEvent *event);

signals:
    /**
     * @brief Сигнал изменения параметров
     * @param hue Значение оттенка
     * @param saturation Значение насыщенности
     * @param value Значение яркости
     */
    void parametersChanged(int hue, int saturation, int value);

    /**
     * @brief Сигнал принятия параметров
     * @param hue Значение оттенка
     * @param saturation Значение насыщенности
     * @param value Значение яркости
     */
    void parametersAccepted(int hue, int saturation, int value);

    /**
     * @brief Сигнал изменения насыщенности
     * @param saturation Значение насыщенности
     */
    void saturationChanged(int saturation);

    /**
     * @brief Сигнал изменения оттенка
     * @param hue Значение оттенка
     */
    void hueChanged(int hue);

    /**
     * @brief Сигнал автоматического принятия параметров
     */
    void autoAccepted();

    /**
     * @brief Сигнал отмены изменений
     */
    void CancelMono();

private:
    int initialSaturation = 0; //< Начальное значение насыщенности
    int initialHue = 0; //< Начальное значение оттенка
    int saturationValue; //< Текущее значение насыщенности
    int hueValue; //< Текущее значение оттенка
    Ui::huesaturation *ui; //< Указатель на пользовательский интерфейс
};

#endif // HUESATURATION_H
