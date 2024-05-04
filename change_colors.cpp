#include "change_colors.h"
#include "qevent.h"
#include "ui_change_colors.h"

Change_colors::Change_colors(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Change_colors)
{
    ui->setupUi(this);
    ui->ColorSlider->setRange(0, 360);
    ui->SaturationSlider->setRange(-100, 100);


    InitialColor = ui->ColorSlider->value();
    InitialSaturation = ui->SaturationSlider->value();

    QString gradientColors;
    for (int i = 0; i < 360; ++i) {
        QString color = QColor::fromHsv(i, 255, 255).name();
        gradientColors += QString("stop:%1 %2,").arg(QString::number(i / 360.0)).arg(color);
    }

    QString styleSheet = QString("QSlider::groove:vertical {"
                                 "    border: 1px solid #999999;"
                                 "    width: 10px;"
                                 "    margin: 0 2px;"
                                 "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, %1);"
                                 "}"
                                 "QSlider::handle:vertical {"
                                 "    background-color: #ffffff;"
                                 "    border: 1px solid #5c5c5c;"
                                 "    height: 18px;"
                                 "    margin: 0 -2px;"
                                 "    border-radius: 3px;"
                                 "}").arg(gradientColors);

    ui->ColorSlider->setStyleSheet(styleSheet);
    QIcon icon(":/icons/mainicon.png");
    this->setWindowIcon(icon);
}

Change_colors::~Change_colors()
{
    delete ui;
}

void Change_colors::on_ColorSlider_sliderMoved(int position)
{
    int color = ui->SaturationSlider->value();
    ui->label_2->setText(QString::number(position));
    emit SendColor(color);
}

void Change_colors::on_SaturationSlider_sliderMoved(int position)
{
    int saturation = ui->SaturationSlider->value();
    ui->label_4->setText(QString::number(position));
    emit SendSaturation(saturation);
}

void Change_colors::on_AcceptButton_clicked()
{
    int saturation = ui->SaturationSlider->value();
    int color = ui->ColorSlider->value();
    emit ParametersSend(saturation, color);
    this->close();
}


void Change_colors::on_CancelButton_clicked()
{
    ui->SaturationSlider->setValue(InitialSaturation);
    ui->ColorSlider->setValue(InitialColor);
    emit CancelChange();
    this->close();
}
void Change_colors::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) {
        on_CancelButton_clicked(); // Вызов соответствующего метода при закрытии окна из-за нажатия на крестик
    }
    event->accept(); // Принимаем событие закрытия окна
}

