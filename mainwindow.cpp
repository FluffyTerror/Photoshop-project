#include "mainwindow.h"
#include "huesaturation.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hueSaturationForm = new huesaturation();
    connect(hueSaturationForm, &huesaturation::parametersAccepted, this, &MainWindow::on_MonochromeParametersChanged);
    connect(hueSaturationForm, &huesaturation::autoAccepted, this, &MainWindow::on_MonochromeAuto);
    connect(hueSaturationForm, &huesaturation::parametersChanged, this, &MainWindow::on_MonochromeParametersChanged);
    connect(hueSaturationForm, &huesaturation::CancelMono, this, &MainWindow::on_CancelMono);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Select_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "Image Files (*.png *.jpg *.jpeg)");

    if (!filepath.isEmpty()) {
        // Создать объект QImage динамически
        QImage *image = new QImage();
        if (image->load(filepath)) {
            loadedImage = image;
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
    QPixmap pixmap = QPixmap::fromImage(*loadedImage);

    // Создание QGraphicsPixmapItem с помощью созданного QPixmap
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(pixmapItem);
    ui->graphicsView->fitInView(pixmapItem, Qt::KeepAspectRatio);
}

void MainWindow::on_Monochrome_clicked()
{

    if (ui->graphicsView->scene->items().isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
            return;
        }
    hueSaturationForm->show();

    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return; // Пропускаем выполнение остальной части функции
    }

}



void MainWindow::on_MonochromeParametersChanged(int saturation, int hue)
{



    QImage monochromeImage = loadedImage->copy(); // Создаем копию оригинального изображения

    // Преобразуем копию в монохромное изображение
    for (int y = 0; y < monochromeImage.height(); ++y) {
        for (int x = 0; x < monochromeImage.width(); ++x) {
            QColor originalColor = monochromeImage.pixelColor(x, y);
            int gray = qGray(originalColor.rgb());
            QColor newColor = QColor::fromHsv((hue * 359) / 360, (saturation + 100) * 255 / 200, gray);
            monochromeImage.setPixelColor(x, y, newColor);
        }
    }

    monochromePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(monochromeImage));
    // Добавляем монохромное изображение на сцену и отображаем его
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(monochromePixmapItem);
    ui->graphicsView->fitInView(monochromePixmapItem, Qt::KeepAspectRatio);
    //delete monochromePixmapItem;
}



void MainWindow::on_MonochromeAuto(){



    QImage monochromeImage = loadedImage->copy(); // Создаем копию оригинального изображения

    // Преобразуем копию в монохромное изображение
    for (int y = 0; y < monochromeImage.height(); ++y) {
        for (int x = 0; x < monochromeImage.width(); ++x) {
            QColor color = monochromeImage.pixelColor(x, y);
            int gray = qGray(color.rgb());
            monochromeImage.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }
    monochromePixmapItem=  new QGraphicsPixmapItem(QPixmap::fromImage(monochromeImage));

    // Добавляем монохромное изображение на сцену и отображаем его
    delete ui->graphicsView->scene->items().first();
    ui->graphicsView->scene->addItem(monochromePixmapItem);
    ui->graphicsView->fitInView(monochromePixmapItem, Qt::KeepAspectRatio);
}
