#include "events.h"

Events::Events()
{

}

Custom_View::Custom_View(QWidget *parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
    scene = new QGraphicsScene(this);
    setScene(scene);
    viewport()->installEventFilter(this);
}

void Custom_View::dragEnterEvent(QDragEnterEvent *event)
{
    QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent *>(event);
    if (dragEnterEvent->mimeData()->hasUrls())
    {
        dragEnterEvent->acceptProposedAction();
    }
}

void Custom_View::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void Custom_View::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}

void Custom_View::dropEvent(QDropEvent *event)
{
    if (event->source() == this)
        return;
    QDropEvent *dropEvent = static_cast<QDropEvent *>(event);
    if (dropEvent->mimeData()->hasUrls())
    {
        QList<QUrl> urlList = dropEvent->mimeData()->urls();
        foreach (QUrl url, urlList)
        {
            if (url.isLocalFile())
            {
                QString filePath = url.toLocalFile();
                loadImage(filePath);
                dropEvent->acceptProposedAction();
            }
        }
    }
}

void Custom_View::loadImage(const QString &filename)
{
    QPixmap pixmap(filename);

    if (!pixmap.isNull())
    {

        scene->clear();
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
        scene->addItem(item);
        setSceneRect(item->boundingRect());
        fitInView(item, Qt::KeepAspectRatio);
    }
    else
    {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Failed to load image!"));
    }
}
