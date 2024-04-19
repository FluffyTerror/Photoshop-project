#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "Image Files (*.png *.jpg *.jpeg)");

    if (!filename.isEmpty()) {
        Custom_View *customView = ui->graphicsView;
        customView->loadImage(filename);
    }
    else{

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
