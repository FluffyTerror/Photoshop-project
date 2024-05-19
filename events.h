#ifndef EVENTS_H
#define EVENTS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QWidget>
#include <QMimeData>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

/**
 * @brief Класс для обработки drag&drop загрузки
 */
class Events
{
public:
    /**
     * @brief Конструктор класса Events
     */
    Events();
};

/**
 * @brief Класс для пользовательского представления, поддерживающего перетаскивание изображений
 */
class Custom_View : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса Custom_View
     * @param parent Указатель на родительский виджет. По умолчанию равен nullptr.
     */
    explicit Custom_View(QWidget *parent = nullptr);

    /**
     * @brief Загрузка изображения
     * @param filename Путь к загружаемому файлу изображения
     */
    void loadImage(const QString &filename);
    void resizeEvent(QResizeEvent *event) override;
    // Указатель на графическую сцену
    QGraphicsScene *scene;

protected:
    /**
     * @brief Обработчик события входа перетаскиваемого элемента в область виджета
     * @param event Указатель на событие входа перетаскиваемого элемента
     */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /**
     * @brief Обработчик события выхода перетаскиваемого элемента из области виджета
     * @param event Указатель на событие выхода перетаскиваемого элемента
     */
    void dragLeaveEvent(QDragLeaveEvent *event) override;

    /**
     * @brief Обработчик события перемещения перетаскиваемого элемента в области виджета
     * @param event Указатель на событие перемещения перетаскиваемого элемента
     */
    void dragMoveEvent(QDragMoveEvent *event) override;

    /**
     * @brief Обработчик события "бросания" элемента в область виджета
     * @param event Указатель на событие "бросания" элемента
     */
    void dropEvent(QDropEvent *event) override;

signals:
    /**
     * @brief Сигнал об успешной загрузке изображения
     * @param filepath Путь к загруженному файлу изображения
     */
    void ImageLoaded(const QString &filepath);
};

#endif // EVENTS_H
