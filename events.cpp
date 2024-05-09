#include "events.h"

Events::Events()
{

}

// Конструктор класса Custom_View
Custom_View::Custom_View(QWidget *parent) : QGraphicsView(parent)
{
    // Установка возможности принятия перетаскиваемых элементов
    setAcceptDrops(true);

    // Создание графической сцены и установка ее для виджета QGraphicsView
    scene = new QGraphicsScene(this);
    setScene(scene);

    // Установка фильтра событий для viewport
    viewport()->installEventFilter(this);
}

// Обработчик события входа перетаскиваемого элемента в область виджета
void Custom_View::dragEnterEvent(QDragEnterEvent *event)
{
    QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent *>(event);
    if (dragEnterEvent->mimeData()->hasUrls())
    {
        dragEnterEvent->acceptProposedAction();
    }
}

// Обработчик события выхода перетаскиваемого элемента из области виджета
void Custom_View::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

// Обработчик события перемещения перетаскиваемого элемента в области виджета
void Custom_View::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}

// Обработчик события "бросания" элемента в область виджета
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

// Загрузка изображения
void Custom_View::loadImage(const QString &filename)
{
    QPixmap pixmap(filename);

    // Проверка, удалось ли загрузить изображение
    if (!pixmap.isNull())
    {
        // Очистка сцены и добавление нового элемента
        scene->clear();
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
        scene->addItem(item);

        // Установка области просмотра и подгонка изображения под виджет
        setSceneRect(item->boundingRect());
        fitInView(item, Qt::KeepAspectRatio);

        // Генерация сигнала об успешной загрузке изображения
        emit ImageLoaded(filename);
    }
    else
    {
        // Вывод предупреждения о неудачной загрузке изображения
        QMessageBox::warning(nullptr, tr("Warning"), tr("Failed to load image!"));
    }
}


