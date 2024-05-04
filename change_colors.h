#ifndef CHANGE_COLORS_H
#define CHANGE_COLORS_H

#include <QWidget>

namespace Ui {
class Change_colors;
}

class Change_colors : public QWidget
{
    Q_OBJECT

public:
    explicit Change_colors(QWidget *parent = nullptr);
    ~Change_colors();

private slots:
    void on_ColorSlider_sliderMoved(int position);
    void on_SaturationSlider_sliderMoved(int position);

    void on_AcceptButton_clicked();

    void on_CancelButton_clicked();
    void closeEvent(QCloseEvent *event);

signals:
    void SendColor(int Colors);
    void SendSaturation(int Saturation);
    void ParametersSend(int color, int saturation);
    void CancelChange();
private:
    Ui::Change_colors *ui;
    int InitialColor = 0;
    int InitialSaturation = 0;
};

#endif // CHANGE_COLORS_H
