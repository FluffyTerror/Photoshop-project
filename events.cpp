
#include "events.h"

/**
 * @brief Конструктор класса Events
 */
Events::Events()
{
}

/**
 * @brief Конструктор класса Custom_View
 * @param parent Указатель на родительский виджет. По умолчанию равен nullptr.
 */
Custom_View::Custom_View(QWidget *parent) : QGraphicsView(parent)
{
    // Установка возможности принятия перетаскиваемых элементов
    setAcceptDrops(true);

    // Создание графической сцены и установка ее для виджета QGraphicsView
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    // Установка фильтра событий для viewport
    viewport()->installEventFilter(this);
}

/**
 * @brief Обработчик события входа перетаскиваемого элемента в область виджета
 * @param event Указатель на событие входа перетаскиваемого элемента
 */
void Custom_View::dragEnterEvent(QDragEnterEvent *event)
{
    QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent *>(event);
    if (dragEnterEvent->mimeData()->hasUrls())
    {
        dragEnterEvent->acceptProposedAction();
    }
}

/**
 * @brief Обработчик события выхода перетаскиваемого элемента из области виджета
 * @param event Указатель на событие выхода перетаскиваемого элемента
 */
void Custom_View::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

/**
 * @brief Обработчик события перемещения перетаскиваемого элемента в области виджета
 * @param event Указатель на событие перемещения перетаскиваемого элемента
 */
void Custom_View::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}

/**
 * @brief Обработчик события "бросания" элемента в область виджета
 * @param event Указатель на событие "бросания" элемента
 */
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

/**
 * @brief Загрузка изображения
 * @param filename Путь к загружаемому файлу изображения
 */
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
void Custom_View::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

}
