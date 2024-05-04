#ifndef HUESATURATION_H
#define HUESATURATION_H

#include <QWidget>

namespace Ui {
class huesaturation;
}

class huesaturation : public QWidget
{
    Q_OBJECT

public:
    explicit huesaturation(QWidget *parent = nullptr);
    ~huesaturation();

private slots:
    void on_AutoButton_clicked();
    void on_CancelButton_clicked();
    void on_AcceptButton_clicked();
    void on_HueSlider_sliderMoved(int position);
    void on_SaturationSlider_sliderMoved(int position);

signals:

    void parametersAccepted(int saturation, int hue);
    void saturationChanged(int saturation);
    void hueChanged(int hue);
    void autoAccepted();
    void CancelMono();
private:
    int initialSaturation=0;
    int initialHue=0;
    int saturationValue;
    int hueValue;
    Ui::huesaturation *ui;
};

#endif // HUESATURATION_H
