#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "events.h"
#include "huesaturation.h"
#include "qgraphicsitem.h"
#include <QMainWindow>
#include "cut_image_mod.h"
#include "color_palette.h"
#include <vector>
#include <QToolBar>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief MainWindow класс
 * Этот класс показывает основные части приложения
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор MainWindow
     * @param parent Указатель на родительский виджет
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор для MainWindow
     */
    ~MainWindow();

    QGraphicsPixmapItem *monochromePixmapItem; ///< Элемент для отображения монохромного изображения
    QGraphicsPixmapItem *CutPixmapItem; ///< Элемент для отображения обрезанного изображения
    QGraphicsPixmapItem *PalettePixmapItem; ///< Элемент для отображения изображения палитры
    QGraphicsPixmapItem *originalPixmapItem; ///< Элемент для отображения оригинального изображения
    huesaturation *hueSaturationForm; ///< Форма для настройки оттенка и насыщенности
    QImage *loadedImage; ///< Указатель на загруженное изображение
    QImage CopyColorImage, CopyColorImageT; ///< Копии цветного изображения
    Custom_View *customView; ///< Виджет пользовательского представления
    Cut_image_mod *cut_image; ///< Объект для функции обрезки изображения
    color_palette *color_pal; ///< Объект для функции палитры цветов

public slots:
    /**
     * @brief Слот для обработки нажатия кнопки выбора
     */
    void on_Select_clicked();

    /**
     * @brief Слот для обработки нажатия кнопки монохрома
     */
    void on_Monochrome_clicked();

    /**
     * @brief Слот для обработки действия принятия
     */
    void on_Accept();

    /**
     * @brief Слот для обработки автоматической настройки монохрома
     */
    void on_MonochromeAuto();

    /**
     * @brief Слот для обработки действия отмены монохрома
     */
    void on_CancelMono();

    /**
     * @brief Слот для принятия изображения
     * @param filepath Путь к файлу изображения
     */
    void ImageAccept(const QString &filepath);

    /**
     * @brief Слот для обработки нажатия кнопки обрезки
     */
    void on_Cut_clicked();

    /**
     * @brief Слот для изменения размера изображения
     * @param type Тип изменения размера
     * @param num_cropped_pixel Количество обрезанных пикселей
     */
    void change_size_image(short type, int num_cropped_pixel);

    /**
     * @brief Слот для обработки нажатия кнопки обрезки в окне обрезки
     */
    void cut_button_clicked();

    /**
     * @brief Слот для обработки нажатия кнопки закрытия в окне обрезки
     */
    void close_cut_button_clicked();

    /**
     * @brief Слот для обработки нажатия кнопки палитры цветов
     */
    void on_color_pal_clicked();

    /**
     * @brief Слот для обработки нажатия кнопки сохранения
     */
    void on_SaveButton_clicked();

    /**
     * @brief Слот для изменения палитры цветов
     * @param NewColor Новый цвет для установки в палитре
     */
    void changeColorPallete(QColor NewColor);

    /**
     * @brief Слот для обработки нажатия кнопки ОК в окне палитры
     */
    void paletteOkClick();

    /**
     * @brief Слот для обработки нажатия кнопки закрытия в окне палитры
     */
    void paletteCloseClick();

    /**
     * @brief Слот для обработки изменений параметров монохрома
     * @param hue Значение оттенка
     * @param saturation Значение насыщенности
     * @param value Значение яркости
     */
    void on_MonochromeParametersChanged(int hue, int saturation, int value);

    /**
     * @brief Слот для обработки нажатия кнопки цвета в палитре
     * @param OldColor Старый цвет, который нужно заменить
     */
    void clicked_color_button_in_pallete(QColor OldColor);
    void createToolBar(); ///< Метод для создания панели инструментов

signals:
    /**
     * @brief Сигнал, передающий информацию о палитре цветов
     * @param colors_pal Вектор цветов в палитре
     */
    void color_pallete_inf(std::vector<QColor> colors_pal);

    /**
     * @brief Сигнал, разрешающий добавление нового цвета в палитру
     * @param NewColor Новый цвет для добавления
     */
    void color_pallete_allow_adding_a_new_color(QColor NewColor);

private:
    /**
     * @brief Структура для хранения координат и информации о цвете
     */
    struct coord_color {
        int x; ///< X координата
        int y; ///< Y координата
        QColor color; ///< Цвет в координате
    };

    /**
     * @brief Создает структуру coord_color
     * @param x X координата
     * @param y Y координата
     * @param color Цвет в координате
     * @return Созданная структура coord_color
     */
    coord_color make_coord_color(int x, int y, QColor color);

    Ui::MainWindow *ui; ///< Объект пользовательского интерфейса
    void crop_image(QImage cut_image_t, QRect cropRect_t); ///< Функция обрезки изображения
    void colors_sort(); ///< Функция сортировки цветов

    std::vector<coord_color> change_pixel_map_pallete; ///< Вектор для хранения карты пикселей, которые нужно изменить

};

#endif // MAINWINDOW_H
