#include "huesaturation.h"
#include "qevent.h"
#include "ui_huesaturation.h"

huesaturation::huesaturation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::huesaturation)
{
    ui->setupUi(this);
    this->setWindowTitle("Monochrome");
    ui->HueSlider->setRange(0, 359);
    ui->SaturationSlider->setRange(-100,100);
    ui->ValueSlider->setRange(-100,100);
    QIcon icon(":/icons/mainicon.png");
    this->setWindowIcon(icon);
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
                                 "    width: 30px;"
                                 "    margin: -2px 0;"
                                 "    border-radius: 3px;" "}").arg(gradientColors)
                                + QString("QSlider::handle:horizontal {"
                                 "background: white; /* Цвет фона ползунка */"
                                 "border: 2px solid gray; /* Цвет рамки ползунка */"
                                 "width: 18px; /* Ширина ползунка */"
                                     "margin-top: -2px; /* Отступ сверху */"
                                     "margin-bottom: -2px; /* Отступ снизу */"
                                     "border-radius: 9px; /* Радиус скругления углов */"
                             "}"
);

    ui->HueSlider->setStyleSheet(styleSheet);


}

huesaturation::~huesaturation()
{
    delete ui;
}



void huesaturation::on_CancelButton_clicked()
{
    ui->SaturationSlider->setValue(initialSaturation);
    ui->HueSlider->setValue(initialHue);
    emit CancelMono();
    this->close();
}

void huesaturation::on_AcceptButton_clicked()
{


    // Передаем значения насыщенности и оттенка через сигнал
    emit parametersAccepted(ui->HueSlider->value(),ui->SaturationSlider->value(),ui->ValueSlider->value());

    this->close();

}





void huesaturation::on_AutoButton_clicked()
{
    emit autoAccepted();
    this->close();
}


void huesaturation::on_HueSlider_sliderMoved(int position)
{
    ui->label_4->setText(QString::number(position));
    emit parametersChanged(position,ui->SaturationSlider->value(),ui->ValueSlider->value());
}


void huesaturation::on_SaturationSlider_sliderMoved(int position)
{
    ui->label_3->setText(QString::number(position));
    emit parametersChanged(ui->HueSlider->value(),position,ui->ValueSlider->value());
}




void huesaturation::on_ValueSlider_sliderMoved(int position)
{
     ui->label_9->setText(QString::number(position));
    emit parametersChanged(ui->HueSlider->value(),ui->SaturationSlider->value(),position);
}

void huesaturation::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) {
        on_CancelButton_clicked(); // Вызов соответствующего метода при закрытии окна из-за нажатия на крестик
    }
    event->accept(); // Принимаем событие закрытия окна
}

