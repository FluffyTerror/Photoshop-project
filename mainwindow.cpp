#include "mainwindow.h"
#include "huesaturation.h"
#include "ui_mainwindow.h"
#include "events.h"
#include "cut_image_mod.h"
#include "type_function.h"

    struct cut_struct
{
    int  num_cropped_pixel_x = 0, num_cropped_pixel_y = 0, num_cropped_pixel_x2 = 0, num_cropped_pixel_y2 = 0;
}cropped;
cut_struct old_cropped;
bool firstCopyImage = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Paint");
    this->setStatusBar(nullptr);
    QIcon icon(":/icons/mainicon.png"); // Путь к вашей иконке
    this->setWindowIcon(icon);


    hueSaturationForm = new huesaturation();
    cut_image = new Cut_image_mod();
    color_pal = new color_palette();
    // connect(hueSaturationForm, &huesaturation::parametersAccepted, this, &MainWindow::on_MonochromeParametersChanged);
    connect(hueSaturationForm, &huesaturation::autoAccepted, this, &MainWindow::on_MonochromeAuto);
    //connect(hueSaturationForm, &huesaturation::parametersChanged, this, &MainWindow::on_MonochromeParametersChanged);
    connect(hueSaturationForm, &huesaturation::CancelMono, this, &MainWindow::on_CancelMono);
    connect(cut_image, &Cut_image_mod::change_slider_position, this, &MainWindow::on_change_size_image);
    connect(cut_image, &Cut_image_mod::cut_image_ok_press, this, &MainWindow::on_cut_button_clicked);
    connect(cut_image, &Cut_image_mod::cut_image_close_press, this, &MainWindow::close_cut_button_clicked);
    connect(this, &MainWindow::color_pallete_inf, color_pal, &color_palette::auto_color_pal);
    connect(this, &MainWindow::color_pallete_allow_adding_a_new_color, color_pal, &color_palette::slots_color_pallete_allow_adding_a_new_color);
    connect(ui->graphicsView, &Custom_View::ImageLoaded, this, &MainWindow::ImageAccept);
    connect(hueSaturationForm, &huesaturation::parametersAccepted, this, &MainWindow::on_Accept);
    connect(hueSaturationForm, &huesaturation::parametersChanged, this, &MainWindow::on_MonochromeParametersChanged);
    connect(color_pal, &color_palette::changeColorPaletteImage, this, &MainWindow::changeColorPallete);
    connect(color_pal, &color_palette::paletteOkClick, this, &MainWindow::paletteOkClick);
    connect(color_pal, &color_palette::paletteCloseClick, this, &MainWindow::paletteCloseClick);

}

void MainWindow::crop_image (QImage cut_image_t, QRect cropRect_t)
{
    cut_image_t = CopyColorImage.copy(cropRect_t);
    CutPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(cut_image_t));
    delete ui->graphicsView->scene->items().value(0);
    ui->graphicsView->scene->addItem(CutPixmapItem);
    ui->graphicsView->setSceneRect(CutPixmapItem->boundingRect());
    ui->graphicsView->fitInView(CutPixmapItem, Qt::KeepAspectRatio);
}


void MainWindow::colors_sort()
{
    QImage cut_image = CopyColorImage.copy();
    std::vector<QColor> colors;
    std::vector<std::pair<QColor, int>> Map_colors;
    int saturn = cut_image.pixelColor(0, 0).saturation();
    for (int y = 0; y < cut_image.height(); y+=10) {
        for (int x = 0; x < cut_image.width();x += 10) {
            saturn = (cut_image.pixelColor(x, y).saturation() + saturn) / 2;
        }
    }
    for (int y = 0; y < cut_image.height(); y+=10) {
        for (int x = 0; x < cut_image.width();x += 10) {
            QColor pixelColor = cut_image.pixelColor(x, y);
            bool no_color = 1;
            saturn = (saturn + pixelColor.saturation())/2;
            for (std::size_t i = 0; i < Map_colors.size(); i+=1)
            {
                if(Map_colors[i].first.red() - 40 <= pixelColor.red() && Map_colors[i].first.red() + 40 >= pixelColor.red() &&
                    Map_colors[i].first.green() - 40 <= pixelColor.green() && Map_colors[i].first.green() + 40 >= pixelColor.green() &&
                    Map_colors[i].first.blue() - 40 <= pixelColor.blue() && Map_colors[i].first.blue() + 40 >= pixelColor.blue())
                {
                    Map_colors[i].second ++;
                    no_color = 0;
                    break;
                }

            }
            if (no_color && pixelColor.saturation() >= saturn )
            {
                Map_colors.push_back(std::make_pair(pixelColor, 1));
            }
        }
    }
    std::sort(Map_colors.begin(), Map_colors.end(), [](const auto &left, const auto &right) {
        return left.second > right.second;
    });
    for (int i = 0; i < Map_colors.size(); i++)
    {
        colors.push_back(Map_colors[i].first);
    }
    emit color_pallete_inf(colors);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete hueSaturationForm;
    delete cut_image;
}


void MainWindow::ImageAccept(const QString &filepath){
    QImage *image = new QImage();
    if (image->load(filepath)) {
        loadedImage = image;
        CopyColorImage = loadedImage->copy();
        GlobalImageCopy = loadedImage->copy();

    }
}

void MainWindow::on_Select_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "Image Files (*.png *.jpg *.jpeg)");

    if (!filepath.isEmpty()) {
        // Создать объект QImage динамически
        QImage *image = new QImage();
        if (image->load(filepath)) {
            loadedImage = image;
            CopyColorImage = loadedImage->copy();
            // Загрузить изображение в Custom_View для отображения
            Custom_View *customView = ui->graphicsView;
            customView->loadImage(filepath);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Failed to load image from file!"));
            delete image; // Освободить память, если загрузка изображения не удалась
        }
    }
}

void MainWindow::on_Accept(){
    QGraphicsItem *item = ui->graphicsView->scene->items().first();

    QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);

    *loadedImage = pixmapItem->pixmap().toImage();
}


void MainWindow::on_CancelMono(){
    // Преобразование QImage в QPixmap
    QImage cut_image = CopyColorImage.copy();
    int w = cut_image.width() - ((cropped.num_cropped_pixel_x2 > cut_image.width())? cut_image.width() : cropped.num_cropped_pixel_x2) - cropped.num_cropped_pixel_x;
    int h = cut_image.height() - ((cropped.num_cropped_pixel_y2 > cut_image.height())? cut_image.height() : cropped.num_cropped_pixel_y2) - cropped.num_cropped_pixel_y;
    QRect cropRect(cropped.num_cropped_pixel_x, cropped.num_cropped_pixel_y, w, h);
    QPixmap pixmap = QPixmap::fromImage(loadedImage->copy(cropRect));

    // Создание QGraphicsPixmapItem с помощью созданного QPixmap
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(pixmapItem);
    ui->graphicsView->setSceneRect(pixmapItem->boundingRect());
    ui->graphicsView->fitInView(pixmapItem, Qt::KeepAspectRatio);
}

void MainWindow::on_Monochrome_clicked()
{

    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
    hueSaturationForm->show();
    on_MonochromeParametersChanged(0,0,0);
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }

}

void MainWindow::on_MonochromeParametersChanged(int hue,int saturation, int value)
{
    QImage monochromeImage = CopyColorImage; // Создаем копию оригинального изображения
    int gray;
    int result;
    for (int y = 0; y < monochromeImage.height(); ++y) {
        for (int x = 0; x < monochromeImage.width(); ++x) {

            QColor color = monochromeImage.pixelColor(x, y);
            gray = int((qGray(color.rgb())));
            // gray = color.;
            // Преобразуем значения насыщенности и значения в диапазон [0, 255]


            if(value >=0){
                double newValue = static_cast<double>(value)/ 100;
                result = (gray + (newValue) * (255 - gray));
                //result = gray * newValue;
            }
            else{
                double newValue = static_cast<double>(-value)/ 100;
                result = (gray - (newValue) * (gray));
            }
            QColor newColor = QColor::fromHsv(hue,  (saturation + 100) * 255 / 200, result );
            monochromeImage.setPixelColor(x, y, newColor);
        }
    }

    CopyColorImage = monochromeImage.copy();
    QImage cut_image = CopyColorImage.copy();
    int w = cut_image.width() - ((cropped.num_cropped_pixel_x2 > cut_image.width())? cut_image.width() : cropped.num_cropped_pixel_x2) - cropped.num_cropped_pixel_x;
    int h = cut_image.height() - ((cropped.num_cropped_pixel_y2 > cut_image.height())? cut_image.height() : cropped.num_cropped_pixel_y2) - cropped.num_cropped_pixel_y;
    QRect cropRect(cropped.num_cropped_pixel_x, cropped.num_cropped_pixel_y, w, h);
    monochromeImage = monochromeImage.copy(cropRect);
    monochromePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(monochromeImage));
    // Добавляем монохромное изображение на сцену и отображаем его
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(monochromePixmapItem);
    ui->graphicsView->fitInView(monochromePixmapItem, Qt::KeepAspectRatio);
    //delete monochromePixmapItem;
}


void MainWindow::changeColorPallete(QColor NewColor, QColor OldColor)
{
    if (firstCopyImage)
    {
        firstCopyImage = 0;
    }
    QImage PaletteImage = CopyColorImage;
    for (int y = 0; y < PaletteImage.height(); y++) {
        for (int x = 0; x < PaletteImage.width();x++) {
            QColor originalColor = PaletteImage.pixelColor(x, y);
            int val = 0, sat = 0;
            if(originalColor.red() - 40 <= OldColor.red() && originalColor.red() + 40 >= OldColor.red() &&
                originalColor.green() - 40 <= OldColor.green() && originalColor.green() + 40 >= OldColor.green() &&
                originalColor.blue() - 40 <= OldColor.blue() && originalColor.blue() + 40 >= OldColor.blue())
            {
                if (originalColor.value() < 10)
                {
                    sat = 250;
                    val = 30;
                }
                else
                {
                    if (originalColor.value() > 180)
                    {
                        sat = (originalColor.saturation() > 80)? originalColor.saturation() : 80;
                        val = 180;
                    }
                    else
                    {
                        sat = (originalColor.saturation() > 80)? originalColor.saturation() : 80;
                        val = (originalColor.value() < 200)? originalColor.value(): 200;
                    }
                }
                NewColor = QColor::fromHsv(NewColor.hue(), sat , val);
                PaletteImage.setPixelColor(x, y, NewColor);
            }
        }
    }
    CopyColorImage = PaletteImage;
    PalettePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(PaletteImage));
    delete ui->graphicsView->scene->items().value(0);
    ui->graphicsView->scene->addItem(PalettePixmapItem);
    ui->graphicsView->setSceneRect(PalettePixmapItem->boundingRect());
    ui->graphicsView->fitInView(PalettePixmapItem, Qt::KeepAspectRatio);
    color_pallete_allow_adding_a_new_color(NewColor);
}

void MainWindow::paletteOkClick()
{
    firstCopyImage = 1;
}

void MainWindow::paletteCloseClick()
{
    PalettePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(CopyColorImage));
    delete ui->graphicsView->scene->items().value(0);
    ui->graphicsView->scene->addItem(PalettePixmapItem);
    ui->graphicsView->setSceneRect(PalettePixmapItem->boundingRect());
    ui->graphicsView->fitInView(PalettePixmapItem, Qt::KeepAspectRatio);
}

void MainWindow::on_MonochromeAuto(){



    QImage monochromeImage = CopyColorImage; // Создаем копию оригинального изображения

    // Преобразуем копию в монохромное изображение
    for (int y = 0; y < monochromeImage.height(); ++y) {
        for (int x = 0; x < monochromeImage.width(); ++x) {
            QColor color = monochromeImage.pixelColor(x, y);
            int gray = qGray(color.rgb());
            monochromeImage.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }

    int w = CopyColorImage.width() - ((cropped.num_cropped_pixel_x2 > CopyColorImage.width())? CopyColorImage.width() : cropped.num_cropped_pixel_x2) - cropped.num_cropped_pixel_x;
    int h = CopyColorImage.height() - ((cropped.num_cropped_pixel_y2 > CopyColorImage.height())? CopyColorImage.height() : cropped.num_cropped_pixel_y2) - cropped.num_cropped_pixel_y;
    QRect cropRect(cropped.num_cropped_pixel_x, cropped.num_cropped_pixel_y, w, h);
    monochromeImage = monochromeImage.copy(cropRect);
    monochromePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(monochromeImage));

    // Добавляем монохромное изображение на сцену и отображаем его
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(monochromePixmapItem);
    ui->graphicsView->fitInView(monochromePixmapItem, Qt::KeepAspectRatio);
}

void MainWindow::on_Cut_clicked()
{
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
    old_cropped = cropped;
    cut_image->show();
    on_change_size_image(0, 0);
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
}

void MainWindow::on_change_size_image(short type, int position)
{
    QImage cut_image = CopyColorImage;
    switch(type)
    {
    case x_fun_right:
    {
        cropped.num_cropped_pixel_x2 = round((cut_image.width() * position) / 100.0);
        cropped.num_cropped_pixel_x2 *= -1;
        break;
    }
    case x_fun_left:
    {
        cropped.num_cropped_pixel_x = round((cut_image.width() * position) / 100.0);
        break;
    }
    case y_fun_up:
    {
        cropped.num_cropped_pixel_y = round((cut_image.height() * position) / 100.0);
        cropped.num_cropped_pixel_y *= -1;
        break;
    }
    case y_fun_down:
    {
        cropped.num_cropped_pixel_y2 = round((cut_image.height() * position) / 100.0);
        break;
    }
    default:
    {
        break;
    }
    }
    for (int y = 0; y < ((cropped.num_cropped_pixel_y > cut_image.height())? cut_image.height() : cropped.num_cropped_pixel_y); ++y) {
        for (int x = 0; x <  cut_image.width(); ++x) {
            cut_image.setPixelColor(x, y, QColor(Qt::black));
        }
    }
    for (int y = 0; y < cut_image.height(); ++y) {
        for (int x = 0; x < ((cropped.num_cropped_pixel_x > cut_image.width())? cut_image.width() : cropped.num_cropped_pixel_x); ++x) {
            cut_image.setPixelColor(x, y, QColor(Qt::black));
        }
    }

    for (int y = 0; y < cut_image.height(); ++y) {
        for (int x = cut_image.width() - ((cropped.num_cropped_pixel_x2 > cut_image.width())? cut_image.width() : cropped.num_cropped_pixel_x2); x < cut_image.width(); ++x) {
            cut_image.setPixelColor(x, y, QColor(Qt::black));
        }
    }
    for (int y = cut_image.height() - ((cropped.num_cropped_pixel_y2 > cut_image.height())? cut_image.height() : cropped.num_cropped_pixel_y2); y < cut_image.height(); ++y) {
        for (int x = 0; x <  cut_image.width(); ++x) {
            cut_image.setPixelColor(x, y, QColor(Qt::black));
        }
    }
    CutPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(cut_image));
    delete ui->graphicsView->scene->items().value(0);
    ui->graphicsView->scene->addItem(CutPixmapItem);
    ui->graphicsView->setSceneRect(CutPixmapItem->boundingRect());
    ui->graphicsView->fitInView(CutPixmapItem, Qt::KeepAspectRatio);
}

void MainWindow::on_cut_button_clicked()
{
    QImage cut_image = CopyColorImage;
    int w = cut_image.width() - ((cropped.num_cropped_pixel_x2 > cut_image.width())? cut_image.width() : cropped.num_cropped_pixel_x2) - cropped.num_cropped_pixel_x;
    int h = cut_image.height() - ((cropped.num_cropped_pixel_y2 > cut_image.height())? cut_image.height() : cropped.num_cropped_pixel_y2) - cropped.num_cropped_pixel_y;
    QRect cropRect(cropped.num_cropped_pixel_x, cropped.num_cropped_pixel_y, w, h);
    crop_image(cut_image, cropRect);
}

void MainWindow::close_cut_button_clicked()
{
    QImage cut_image = CopyColorImage;
    int w = cut_image.width() - ((old_cropped.num_cropped_pixel_x2 > cut_image.width())? cut_image.width() : old_cropped.num_cropped_pixel_x2) - old_cropped.num_cropped_pixel_x;
    int h = cut_image.height() - ((old_cropped.num_cropped_pixel_y2 > cut_image.height())? cut_image.height() : old_cropped.num_cropped_pixel_y2) - old_cropped.num_cropped_pixel_y;
    QRect cropRect(old_cropped.num_cropped_pixel_x, old_cropped.num_cropped_pixel_y, w, h);
    crop_image(cut_image, cropRect);
    cropped = old_cropped;

}




void MainWindow::on_color_pal_clicked()
{
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
    firstCopyImage = 1;
    colors_sort();
    color_pal->show();
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
}




void MainWindow::on_SaveButton_clicked()
{
    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "C://", tr("Image Files (*.png *.jpg *.jpeg)"));

    if (!filePath.isEmpty())
    {
        QGraphicsItem *item = ui->graphicsView->scene->items().first();

        QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);

        *loadedImage = pixmapItem->pixmap().toImage();
        QImage *saveimage = new QImage();
        *saveimage = loadedImage->copy();

        if (!saveimage->save(filePath)) {
            QMessageBox::warning(this, tr("Error"), tr("Failed to save image"));
            return;
        }

        if (filePath.isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("No file selected"));
            return;
        }
        delete saveimage;
    }
    qDebug() << "Image saved successfully.";
}
