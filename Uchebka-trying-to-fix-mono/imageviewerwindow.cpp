#include "imageviewerwindow.h"
#include "qgraphicsitem.h"
#include "qwidget.h"

ImageViewerWindow::ImageViewerWindow(QWidget *parent) : QMainWindow(parent)
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    setCentralWidget(view);
}

void ImageViewerWindow::loadImage(const QString &filename)
{
    QPixmap pixmap(filename);
    if (!pixmap.isNull())
    {
        scene->clear();
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
        scene->addItem(item);
        view->fitInView(item, Qt::KeepAspectRatio);
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Failed to load image!"));
    }
}
