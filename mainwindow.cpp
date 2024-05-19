
#include "mainwindow.h"
#include "huesaturation.h"
#include "ui_mainwindow.h"
#include "events.h"
#include "cut_image_mod.h"
#include "type_function.h"

/**
 * @brief Структура для хранения размеров обрезанного изображения
 */
struct cut_struct {
    int num_cropped_pixel_x = 0; //< Количество обрезанных пикселей по оси X слева
    int num_cropped_pixel_y = 0; //< Количество обрезанных пикселей по оси Y сверху
    int num_cropped_pixel_x2 = 0; //< Количество обрезанных пикселей по оси X справа
    int num_cropped_pixel_y2 = 0; //< Количество обрезанных пикселей по оси Y снизу
} cropped;

cut_struct old_cropped;

/**
 * @brief Функция для обрезки изображения на основе структуры cut_struct
 * @param Image Изображение, которое нужно обрезать
 * @return Прямоугольник, определяющий область обрезанного изображения
 */
QRect CropImage_for_scene(QImage Image) {
    int w = Image.width() - ((cropped.num_cropped_pixel_x2 > Image.width()) ? Image.width() : cropped.num_cropped_pixel_x2) - cropped.num_cropped_pixel_x;
    int h = Image.height() - ((cropped.num_cropped_pixel_y2 > Image.height()) ? Image.height() : cropped.num_cropped_pixel_y2) - cropped.num_cropped_pixel_y;
    QRect cropRect(cropped.num_cropped_pixel_x, cropped.num_cropped_pixel_y, w, h);
    return cropRect;
}

/**
 * @brief Конструктор главного окна
 * @param parent Родительский виджет
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->graphicsView);
    // Устанавливаем заголовок окна
    this->setWindowTitle("Paint");
    // Отключаем статусную строку
    this->setStatusBar(nullptr);
    // Устанавливаем иконку приложения
    QIcon icon(":/icons/mainicon.png");
    this->setWindowIcon(icon);

    // Создаем объекты форм для различных окон
    hueSaturationForm = new huesaturation();
    cut_image = new Cut_image_mod();
    color_pal = new color_palette();

    // Привязка сигналов из других окон к слотам в MainWindow
    connect(hueSaturationForm, &huesaturation::autoAccepted, this, &MainWindow::on_MonochromeAuto);
    connect(hueSaturationForm, &huesaturation::CancelMono, this, &MainWindow::on_CancelMono);
    connect(cut_image, &Cut_image_mod::change_slider_position, this, &MainWindow::change_size_image);
    connect(cut_image, &Cut_image_mod::cut_image_ok_press, this, &MainWindow::cut_button_clicked);
    connect(cut_image, &Cut_image_mod::cut_image_close_press, this, &MainWindow::close_cut_button_clicked);
    connect(this, &MainWindow::color_pallete_inf, color_pal, &color_palette::auto_color_pal);
    connect(this, &MainWindow::color_pallete_allow_adding_a_new_color, color_pal, &color_palette::slots_color_pallete_allow_adding_a_new_color);
    connect(ui->graphicsView, &Custom_View::ImageLoaded, this, &MainWindow::ImageAccept);
    connect(hueSaturationForm, &huesaturation::parametersAccepted, this, &MainWindow::on_Accept);
    connect(hueSaturationForm, &huesaturation::parametersChanged, this, &MainWindow::on_MonochromeParametersChanged);
    connect(color_pal, &color_palette::changeColorPaletteImage, this, &MainWindow::changeColorPallete);
    connect(color_pal, &color_palette::paletteOkClick, this, &MainWindow::paletteOkClick);
    connect(color_pal, &color_palette::paletteCloseClick, this, &MainWindow::paletteCloseClick);
    connect(color_pal, &color_palette::color_button_clicked, this, &MainWindow::clicked_color_button_in_pallete);
    createToolBar();
}

void MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar(tr("Инструменты"));

    QAction *selectAction = new QAction(tr("Выбрать"), this);
    connect(selectAction, &QAction::triggered, this, &MainWindow::on_Select_clicked);
    toolBar->addAction(selectAction);

    QAction *monochromeAction = new QAction(tr("Монохром"), this);
    connect(monochromeAction, &QAction::triggered, this, &MainWindow::on_Monochrome_clicked);
    toolBar->addAction(monochromeAction);

    QAction *cutAction = new QAction(tr("Обрезать"), this);
    connect(cutAction, &QAction::triggered, this, &MainWindow::on_Cut_clicked);
    toolBar->addAction(cutAction);

    QAction *paletteAction = new QAction(tr("Палитра"), this);
    connect(paletteAction, &QAction::triggered, this, &MainWindow::on_color_pal_clicked);
    toolBar->addAction(paletteAction);

    QAction *saveAction = new QAction(tr("Сохранить"), this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::on_SaveButton_clicked);
    toolBar->addAction(saveAction);
}
/**
 * @brief Слот, обрабатывающий нажатие клавиши загрузки изображения
 */
void MainWindow::on_Select_clicked()
{
    // Открываем окно с проводником для выбора загружаемого изображения
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "Image Files (*.png *.jpg *.jpeg)");
    // Устанавливаем нули в параметры обрезанных пикселей
    cropped.num_cropped_pixel_x = 0;
    cropped.num_cropped_pixel_x2 = 0;
    cropped.num_cropped_pixel_y = 0;
    cropped.num_cropped_pixel_y2 = 0;

    if (!filepath.isEmpty()) {
        // При корректном чтении изображения из проводника выводим его на сцену
        QImage *image = new QImage();
        if (image->load(filepath)) {
            CopyColorImage = *image;
            // Загрузить изображение в Custom_View для отображения
            Custom_View *customView = ui->graphicsView;
            customView->loadImage(filepath);
        } else {
            // При неудаче выводится ошибка
            QMessageBox::warning(this, tr("Error"), tr("Failed to load image from file!"));
            delete image; // Освободить память, если загрузка изображения не удалась
        }
    }
}

/**
 * @brief Слот для создания копии изображения, реагирует при выводе объекта на сцену
 * @param filepath Путь к загруженному файлу
 */
void MainWindow::ImageAccept(const QString &filepath) {
    QImage *image = new QImage();
    if (image->load(filepath)) {
        CopyColorImage = image->copy();
        cropped.num_cropped_pixel_x = 0;
        cropped.num_cropped_pixel_x2 = 0;
        cropped.num_cropped_pixel_y = 0;
        cropped.num_cropped_pixel_y2 = 0;
    }
}

/**
 * @brief Обработка нажатия кнопки "Монохром"
 */
void MainWindow::on_Monochrome_clicked()
{
    // Проверка, есть ли изображение на сцене
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
    // Отображение окна применения эффектов для тонирования
    hueSaturationForm->show();
    this->setEnabled(false);
    // Применение нейтральных параметров к изображению
    on_MonochromeParametersChanged(0, 0, 0);
}

/**
 * @brief Функция, отвечающая за применение параметров из окна Монохром
 * @param hue Значение оттенка
 * @param saturation Значение насыщенности
 * @param value Значение яркости
 */
void MainWindow::on_MonochromeParametersChanged(int hue, int saturation, int value)
{
    QImage monochromeImage = CopyColorImage.copy(); // Создаем копию оригинального изображения

    int gray;
    int result;
    QColor color;
    QColor newColor;
    double newValue;

    // Начинаем перебирать каждый пиксель изображения
    for (int y = 0; y < monochromeImage.height(); ++y) {
        for (int x = 0; x < monochromeImage.width(); ++x) {
            color = monochromeImage.pixelColor(x, y); // Запоминаем цвет оригинального пикселя
            gray = int((qGray(color.rgb()))); // Получаем его монохромную версию

            if (value >= 0) { // Если значение яркости >= нуля, используем равномерно распределенную формулу для увеличения яркости
                newValue = static_cast<double>(value) / 100;
                result = (gray + (newValue) * (255 - gray));
            } else { // Формула для уменьшения яркости
                newValue = static_cast<double>(-value) / 100;
                result = (gray - (newValue) * (gray));
            }
            // Получаем новый цвет путем установки полученных параметров
            newColor = QColor::fromHsv(hue, (saturation + 100) * 255 / 200, result);
            // Устанавливаем новый цвет пикселю
            monochromeImage.setPixelColor(x, y, newColor);
        }
    }
    CopyColorImageT = monochromeImage.copy();
    monochromeImage = monochromeImage.copy(CropImage_for_scene(CopyColorImageT));

    // Приводим изображение к нужному типу данных
    monochromePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(monochromeImage));
    // Добавляем монохромное изображение на сцену и отображаем его, удаляем предыдущее
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(monochromePixmapItem);
    ui->graphicsView->fitInView(monochromePixmapItem, Qt::KeepAspectRatio);
}

/**
 * @brief Слот для обработки нажатия кнопки "Авто" в режиме работы с монохромом
 */
void MainWindow::on_MonochromeAuto() {
    QImage monochromeImage = CopyColorImage.copy(); // Создаем копию оригинального изображения
    QColor color;
    int gray;
    // Преобразуем копию в монохромное изображение только с заранее установленными параметрами
    for (int y = 0; y < monochromeImage.height(); ++y) {
        for (int x = 0; x < monochromeImage.width(); ++x) {
            color = monochromeImage.pixelColor(x, y);
            gray = qGray(color.rgb());
            monochromeImage.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }
    CopyColorImage = monochromeImage.copy();
    monochromeImage = monochromeImage.copy(CropImage_for_scene(CopyColorImage));
    monochromePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(monochromeImage));
    // Добавляем монохромное изображение на сцену и отображаем его
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(monochromePixmapItem);
    ui->graphicsView->fitInView(monochromePixmapItem, Qt::KeepAspectRatio);
    this->setEnabled(true);
}

/**
 * @brief Обработка кнопки "Принять" в окне Монохром
 */
void MainWindow::on_Accept() {
    CopyColorImage = CopyColorImageT.copy();
    this->setEnabled(true);
}

/**
 * @brief Обработка нажатия кнопки "Отмена" при работе в режиме монохром
 */
void MainWindow::on_CancelMono() {
    // Преобразование QImage в QPixmap
    QImage cut_image = CopyColorImage.copy();
    QPixmap pixmap = QPixmap::fromImage(CopyColorImage.copy(CropImage_for_scene(cut_image)));

    // Создание QGraphicsPixmapItem с помощью созданного QPixmap
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(pixmapItem);
    ui->graphicsView->setSceneRect(pixmapItem->boundingRect());
    // Установка исходного изображения до применения тонирования
    ui->graphicsView->fitInView(pixmapItem, Qt::KeepAspectRatio);
    this->setEnabled(true);
}

/**
 * @brief Функция для вывода обрезанного изображения
 * @param cut_image_t Обрезанное изображение
 * @param cropRect_t Область обрезки
 */
void MainWindow::crop_image(QImage cut_image_t, QRect cropRect_t) {
    cut_image_t = CopyColorImage.copy(cropRect_t);
    CutPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(cut_image_t));
    delete ui->graphicsView->scene->items().value(0);
    ui->graphicsView->scene->addItem(CutPixmapItem);
    ui->graphicsView->setSceneRect(CutPixmapItem->boundingRect());
    ui->graphicsView->fitInView(CutPixmapItem, Qt::KeepAspectRatio);
}

/**
 * @brief Обработка нажатия кнопки "Обрезать"
 */
void MainWindow::on_Cut_clicked() {
    // Проверка на наличие изображения
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
    this->setEnabled(false);
    old_cropped = cropped;

    // Отображение окна обрезания
    cut_image->show();
    change_size_image(0, 0);
}

/**
 * @brief Обработка движения ползунков в окне для обрезки изображения
 * @param type Тип обрезки
 * @param position Позиция ползунка
 */
void MainWindow::change_size_image(short type, int position) {
    QImage cut_image = CopyColorImage.copy();

    // Вычисляется процент фото, который нужно обрезать и с какой стороны
    switch (type) {
    case x_fun_right: {
        cropped.num_cropped_pixel_x2 = round((cut_image.width() * position) / 100.0);
        cropped.num_cropped_pixel_x2 *= -1;
        break;
    }
    case x_fun_left: {
        cropped.num_cropped_pixel_x = round((cut_image.width() * position) / 100.0);
        break;
    }
    case y_fun_up: {
        cropped.num_cropped_pixel_y = round((cut_image.height() * position) / 100.0);
        cropped.num_cropped_pixel_y *= -1;
        break;
    }
    case y_fun_down: {
        cropped.num_cropped_pixel_y2 = round((cut_image.height() * position) / 100.0);
        break;
    }
    default:
        break;
    }

    // Замена обрезанных пикселей для визуального отображения обрезки
    for (int y = 0; y < ((cropped.num_cropped_pixel_y > cut_image.height()) ? cut_image.height() : cropped.num_cropped_pixel_y); ++y) {
        for (int x = 0; x < cut_image.width(); ++x) {
            cut_image.setPixelColor(x, y, QColor(Qt::white));
        }
    }
    for (int y = 0; y < cut_image.height(); ++y) {
        for (int x = 0; x < ((cropped.num_cropped_pixel_x > cut_image.width()) ? cut_image.width() : cropped.num_cropped_pixel_x); ++x) {
            cut_image.setPixelColor(x, y, QColor(Qt::white));
        }
    }

    for (int y = 0; y < cut_image.height(); ++y) {
        for (int x = cut_image.width() - ((cropped.num_cropped_pixel_x2 > cut_image.width()) ? cut_image.width() : cropped.num_cropped_pixel_x2); x < cut_image.width(); ++x) {
            cut_image.setPixelColor(x, y, QColor(Qt::white));
        }
    }
    for (int y = cut_image.height() - ((cropped.num_cropped_pixel_y2 > cut_image.height()) ? cut_image.height() : cropped.num_cropped_pixel_y2); y < cut_image.height(); ++y) {
        for (int x = 0; x < cut_image.width(); ++x) {
            cut_image.setPixelColor(x, y, QColor(Qt::white));
        }
    }

    // Установка нового обрезанного изображения на сцену
    CutPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(cut_image));
    delete ui->graphicsView->scene->items().value(0);
    ui->graphicsView->scene->addItem(CutPixmapItem);
    ui->graphicsView->setSceneRect(CutPixmapItem->boundingRect());
    ui->graphicsView->fitInView(CutPixmapItem, Qt::KeepAspectRatio);
}



/**
 * @brief Обработка нажатия кнопки обрезать в окне обрезки
 */
void MainWindow::cut_button_clicked()
{
    this->setEnabled(true);
    int w =  CopyColorImage.width() - ((cropped.num_cropped_pixel_x2 >  CopyColorImage.width())?  CopyColorImage.width() : cropped.num_cropped_pixel_x2) - cropped.num_cropped_pixel_x;
    int h =  CopyColorImage.height() - ((cropped.num_cropped_pixel_y2 >  CopyColorImage.height())?  CopyColorImage.height() : cropped.num_cropped_pixel_y2) - cropped.num_cropped_pixel_y;
    QRect cropRect(cropped.num_cropped_pixel_x, cropped.num_cropped_pixel_y, w, h);
    //вызов функции для сохранения обрезанного изображения
    crop_image( CopyColorImage, cropRect);
}

/**
 * @brief Обработка кнопки отмена в режиме обрезки
 */
void MainWindow::close_cut_button_clicked()
{
    this->setEnabled(true);
    int w =  CopyColorImage.width() - ((old_cropped.num_cropped_pixel_x2 >  CopyColorImage.width())?  CopyColorImage.width() : old_cropped.num_cropped_pixel_x2) - old_cropped.num_cropped_pixel_x;
    int h =  CopyColorImage.height() - ((old_cropped.num_cropped_pixel_y2 >  CopyColorImage.height())?  CopyColorImage.height() : old_cropped.num_cropped_pixel_y2) - old_cropped.num_cropped_pixel_y;
    QRect cropRect(old_cropped.num_cropped_pixel_x, old_cropped.num_cropped_pixel_y, w, h);
    crop_image( CopyColorImage, cropRect);
    cropped = old_cropped;

}

/**
 * @brief Обработка нажатия кнопки Палитра
 */
void MainWindow::on_color_pal_clicked()
{
    //Проверка на наличие изображения
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
    CopyColorImageT = CopyColorImage.copy();
    this->setEnabled(false);

    //Запуск алгоритма вычисления палитры и отображения окна
    colors_sort();
    color_pal->show();

}



/**
 * @brief Алгоритм вычиления палитры цветов
 */
void MainWindow::colors_sort()
{
    // Создаем копию изображения
    QImage cut_image = CopyColorImageT.copy();

    // Вектор для хранения цветов
    std::vector<QColor> colors;

    // Вектор для хранения пар цветов и количества пикселей этого цвета
    std::vector<std::pair<QColor, int>> Map_colors;

    // Начальное значение насыщенности цвета
    int saturn = cut_image.pixelColor(0, 0).saturation();

    // Вычисляем среднее значение насыщенности по изображению
    for (int y = 0; y < cut_image.height(); y += 10) {
        for (int x = 0; x < cut_image.width(); x += 10) {
            saturn = (cut_image.pixelColor(x, y).saturation() + saturn) / 2;
        }
    }

    // Проходимся по каждому пикселю изображения с шагом 10x10
    for (int y = 0; y < cut_image.height(); y += 10) {
        for (int x = 0; x < cut_image.width(); x += 10) {
            // Получаем цвет пикселя
            QColor pixelColor = cut_image.pixelColor(x, y);

            // Переменная для отслеживания наличия цвета в Map_colors
            bool no_color = true;

            // Обновляем среднее значение насыщенности
            saturn = (saturn + pixelColor.saturation()) / 2;

            for (std::size_t i = 0; i < Map_colors.size(); i++) {
                if (Map_colors[i].first.red() - 40 <= pixelColor.red() &&
                    Map_colors[i].first.red() + 40 >= pixelColor.red() &&
                    Map_colors[i].first.green() - 40 <= pixelColor.green() &&
                    Map_colors[i].first.green() + 40 >= pixelColor.green() &&
                    Map_colors[i].first.blue() - 40 <= pixelColor.blue() &&
                    Map_colors[i].first.blue() + 40 >= pixelColor.blue()) {
                    Map_colors[i].second++;
                    no_color = false;
                    break;
                }
            }

            // Если цвет не был найден и насыщенность пикселя больше или равна средней насыщенности
            if (no_color && pixelColor.saturation() >= saturn) {
                Map_colors.push_back(std::make_pair(pixelColor, 1));
            }
        }
    }

    // Сортируем Map_colors в порядке убывания количества пикселей
    std::sort(Map_colors.begin(), Map_colors.end(), [](const auto &left, const auto &right) {
        return left.second > right.second;
    });

    // Добавляем цвета в вектор colors
    for (int i = 0; i < Map_colors.size(); i++) {
        colors.push_back(Map_colors[i].first);
    }

    // Вызываем сигнал, передавая полученные цвета
    emit color_pallete_inf(colors);
}

/**
 * @brief метод класса для заполнения внутреней структуры класса mainwindow
 * @param x - ширина
 * @param y - высота
 * @param color цвет
 * @return заполненная структура
 */
MainWindow::coord_color MainWindow::make_coord_color(int x, int y, QColor color)
{
    coord_color Tcolor;
    Tcolor.color = color;
    Tcolor.x = x;
    Tcolor.y = y;
    return Tcolor;
}

/**
 * @brief Обработка кнопки ОК в окне палитры
 */
void MainWindow::paletteOkClick()
{
    CopyColorImage = CopyColorImageT.copy();
    this->setEnabled(true);
}

/**
 * @brief Обработка кнопки Отмена в окне палитры
 */
void MainWindow::paletteCloseClick()
{
    QImage CopyImageCut = CopyColorImage.copy();
    CopyImageCut = CopyImageCut.copy(CropImage_for_scene(CopyImageCut));
    PalettePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(CopyImageCut));
    delete ui->graphicsView->scene->items().value(0);
    ui->graphicsView->scene->addItem(PalettePixmapItem);
    ui->graphicsView->setSceneRect(PalettePixmapItem->boundingRect());
    ui->graphicsView->fitInView(PalettePixmapItem, Qt::KeepAspectRatio);
    this->setEnabled(true);
}

/**
 * @brief Mетод, который заносит в vector координаты пикселя, цвет которого нужно поменять
 * @param OldColor старый цвет
 */
void MainWindow::clicked_color_button_in_pallete(QColor OldColor)
{
    QImage PaletteImage = CopyColorImageT.copy();
    change_pixel_map_pallete.clear();
    for (int y = 0; y < PaletteImage.height(); y++) {
        for (int x = 0; x < PaletteImage.width(); x++) {
            // Получаем оригинальный цвет пикселя
            QColor originalColor = PaletteImage.pixelColor(x, y);

            // Проверяем, попадает ли оригинальный цвет в диапазон старого цвета
            if (originalColor.red() - 40 <= OldColor.red() &&
                originalColor.red() + 40 >= OldColor.red() &&
                originalColor.green() - 40 <= OldColor.green() &&
                originalColor.green() + 40 >= OldColor.green() &&
                originalColor.blue() - 40 <= OldColor.blue() &&
                originalColor.blue() + 40 >= OldColor.blue())
            {
                // заносим в массив значения координат пикселя и данные о его цвете
                change_pixel_map_pallete.push_back(make_coord_color(x, y, originalColor));
            }
        }
    }
}

/**
 * @brief  Алгоритм изменения палитры изображения
 * @param NewColor новый цвет
 */
void MainWindow::changeColorPallete(QColor NewColor)
{
    // Создаем копию изображения палитры
    QImage PaletteImage = CopyColorImageT.copy();

    // Проходимся по каждому пикселю изображения через массив, хранящий координаты пикселей
    //и какой цвет находится в этом пикселе
    for (int i = 0; i < change_pixel_map_pallete.size(); i++)
    {
        // Переменные для хранения значений яркости и насыщенности цвета
        int val = 0, sat = 0;
        // Если яркость оригинального цвета меньше 10
        if (change_pixel_map_pallete[i].color.value() < 10)
        {
            sat = 250;
            val = 35;
        }
        else
        {
            // Если яркость оригинального цвета больше или равна 200
            if (change_pixel_map_pallete[i].color.value() >= 200)
            {
                sat = (change_pixel_map_pallete[i].color.saturation() > 80) ? change_pixel_map_pallete[i].color.saturation() : 80;
                val = 200;
            }
            else
            {
                sat = (change_pixel_map_pallete[i].color.saturation() > 80) ? change_pixel_map_pallete[i].color.saturation() : 80;
                val = change_pixel_map_pallete[i].color.value();
            }
        }
        // Преобразуем новый цвет в HSV-пространство с соответствующей насыщенностью и яркостью
        NewColor = QColor::fromHsv(NewColor.hue(), sat, val);

        // Устанавливаем новый цвет пикселя
        PaletteImage.setPixelColor(change_pixel_map_pallete[i].x, change_pixel_map_pallete[i].y, NewColor);
    }
    // Обновляем копию изображения палитры
    CopyColorImageT = PaletteImage.copy();

    // Обрезаем изображение
    PaletteImage = PaletteImage.copy(CropImage_for_scene(CopyColorImageT));

    // Создаем элемент графического изображения из обновленной палитры
    PalettePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(PaletteImage));

    // Удаляем предыдущий элемент из сцены
    delete ui->graphicsView->scene->items().value(0);

    // Добавляем новый элемент в сцену
    ui->graphicsView->scene->addItem(PalettePixmapItem);

    // Устанавливаем размер сцены и вида изображения
    ui->graphicsView->setSceneRect(PalettePixmapItem->boundingRect());
    ui->graphicsView->fitInView(PalettePixmapItem, Qt::KeepAspectRatio);

    // Генерируем сигнал для разрешения добавления нового цвета
    emit color_pallete_allow_adding_a_new_color(NewColor);

}


/**
 * @brief Обработчик нажатия кнопки "Save"
 */
void MainWindow::on_SaveButton_clicked()
{
    // Проверяем, загружено ли изображение
    if (ui->graphicsView->scene->items().isEmpty()) {
        // Если изображение не загружено, выводим предупреждение и завершаем функцию
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }

    // Запрашиваем у пользователя путь для сохранения изображения
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "C://", tr("Image Files (*.png *.jpg *.jpeg)"));

    // Если пользователь не отменил диалог сохранения
    if (!filePath.isEmpty())
    {
        // Получаем первый элемент сцены (изображение)
        QGraphicsItem *item = ui->graphicsView->scene->items().first();

        // Приводим элемент к типу QGraphicsPixmapItem
        QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);

        // Получаем изображение из элемента сцены
        CopyColorImage = pixmapItem->pixmap().toImage();

        // Создаем новый объект изображения для сохранения
        QImage *saveimage = new QImage();
        *saveimage =  CopyColorImage;

        // Сохраняем изображение
        if (!saveimage->save(filePath)) {
            // Если произошла ошибка при сохранении, выводим предупреждение и завершаем функцию
            QMessageBox::warning(this, tr("Error"), tr("Failed to save image"));
            return;
        }

        // Если путь сохранения пустой
        if (filePath.isEmpty()) {
            // Выводим предупреждение о не выбранном файле и завершаем функцию
            QMessageBox::warning(this, tr("Error"), tr("No file selected"));
            return;
        }

        // Удаляем объект изображения после сохранения
        delete saveimage;
    }

    // Выводим сообщение об успешном сохранении изображения
    qDebug() << "Image saved successfully.";
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (ui->graphicsView->scene->items().isEmpty()) {
        return;
    }
    else{
        ui->graphicsView->fitInView(ui->graphicsView->scene->items().first(), Qt::KeepAspectRatio);
    }
}

/**
 * @brief Деструктор
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete hueSaturationForm;
    delete cut_image;
}
