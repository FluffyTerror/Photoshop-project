#ifndef CUT_IMAGE_MOD_H
#define CUT_IMAGE_MOD_H

#include <QWidget>
#include<QCloseEvent>

namespace Ui {
class Cut_image_mod;
}

class Cut_image_mod : public QWidget
{
    Q_OBJECT

public:
    explicit Cut_image_mod(QWidget *parent = nullptr);
    ~Cut_image_mod();
private slots:
    void on_XSlider_sliderMoved(int position);
    void on_X2Slider_sliderMoved(int position);
    void on_YSlider_sliderMoved(int position);
    void on_Y2Slider_sliderMoved(int position);

    void on_cut_ok_clicked();

    void on_cut_cancellation_clicked();

    void closeEvent(QCloseEvent *event);


private:
    Ui::Cut_image_mod *ui;
    int x = 0, y = 0, x2 = 0, y2 = 0; //мне было лень придумывать адекватные названия, по этому поясняю: х - обрезает слева, х2 - справа, у - сверху, у2 - снизу
signals:
    void change_slider_position(short num_type_cut, int num_cropped_pixel);
    void cut_image_ok_press();
    void cut_image_close_press();

};

#endif // CUT_IMAGE_MOD_H
