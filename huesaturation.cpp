#include "huesaturation.h"
#include "ui_huesaturation.h"

huesaturation::huesaturation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::huesaturation)
{
    ui->setupUi(this);
    ui->HueSlider->setRange(0, 360);
    ui->SaturationSlider->setRange(-100,100);

    initialSaturation = ui->SaturationSlider->value();
    initialHue = ui->HueSlider->value();

    QString gradientColors;
    for (int i = 0; i < 360; ++i) {
        QString color = QColor::fromHsv(i, 255, 255).name();
        gradientColors += QString("stop:%1 %2,").arg(QString::number(i / 360.0)).arg(color);
    }

    QString styleSheet = QString("QSlider::groove:horizontal {"
                                 "    border: 1px solid #999999;"
                                 "    height: 10px;"
                                 "    margin: 2px 0;"
                                 "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, %1);"
                                 "}"
                                 "QSlider::handle:horizontal {"
                                 "    background-color: #ffffff;"
                                 "    border: 1px solid #5c5c5c;"
                                 "    width: 18px;"
                                 "    margin: -2px 0;"
                                 "    border-radius: 3px;"
                                 "}").arg(gradientColors);
    ui->HueSlider->setStyleSheet(styleSheet);
}

huesaturation::~huesaturation()
{
    delete ui;
}


void huesaturation::on_SaturationSlider_valueChanged(int value)
{
    ui->label_3->setText(QString::number(value));
    emit saturationChanged(value);//передача аргумента по слоту
}

void huesaturation::on_HueSlider_valueChanged(int value)
{
    ui->label_4->setText(QString::number(value));
    emit hueChanged(value);
}


void huesaturation::on_CancelButton_clicked()
{
    ui->SaturationSlider->setValue(initialSaturation);
    ui->HueSlider->setValue(initialHue);

    this->close();
}

void huesaturation::on_AcceptButton_clicked()
{
    // Сохраняем текущие значения насыщенности и оттенка
    int saturation = ui->SaturationSlider->value();
    int hue = ui->HueSlider->value();

    // Передаем значения насыщенности и оттенка через сигнал
    emit parametersAccepted(saturation, hue);

    this->close();

}

