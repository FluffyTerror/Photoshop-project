#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageviewerwindow.h" // Добавьте это

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{


    // Создаем диалоговое окно выбора файла
    QFileDialog dialog(this, tr("Open File"), "C://", "Image Files (*.png *.jpg *.jpeg)");

    // Отключаем стандартное поведение открытия диалога
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setOption(QFileDialog::DontUseNativeDialog);

    // Если пользователь выбрал файл, отображаем его в новом окне
    if (dialog.exec() == QDialog::Accepted) {
        QStringList filenames = dialog.selectedFiles();
        if (!filenames.isEmpty()) {
            ImageViewerWindow *imageViewerWindow = new ImageViewerWindow;
            imageViewerWindow->loadImage(filenames.first());
            imageViewerWindow->show();
        }
    }
}
//void MainWindow::on_pushButton_clicked()
//{
  //

    //if (!filename.isEmpty()) {
      //  ImageViewerWindow *imageViewerWindow = new ImageViewerWindow;
        //imageViewerWindow->loadImage(filename);
        //imageViewerWindow->show();
    //}
//}
