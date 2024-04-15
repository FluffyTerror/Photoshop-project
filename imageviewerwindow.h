#ifndef IMAGEVIEWERWINDOW_H
#define IMAGEVIEWERWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMessageBox>

class ImageViewerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ImageViewerWindow(QWidget *parent = nullptr);

    void loadImage(const QString &filename);

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
};

#endif // IMAGEVIEWERWINDOW_H
