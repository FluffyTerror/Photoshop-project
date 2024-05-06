
#include "changecolorpalette.h"
#include "ui_changecolorpalette.h"

    changeColorPalette::changeColorPalette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changeColorPalette)
{
    ui->setupUi(this);
    this->setWindowTitle("ChangeColor");
    ui->colorSlide->setRange(0, 360);

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
                                 "}").arg(gradientColors) + QString("QSlider::handle:horizontal {"
                                   "background: white; /* Цвет фона ползунка */"
                                   "border: 2px solid gray; /* Цвет рамки ползунка */"
                                   "width: 18px; /* Ширина ползунка */"
                                   "margin-top: -2px; /* Отступ сверху */"
                                   "margin-bottom: -2px; /* Отступ снизу */"
                                   "border-radius: 9px; /* Радиус скругления углов */"
                                   "}"
                                   );

    ui->colorSlide->setStyleSheet(styleSheet);
}

changeColorPalette::~changeColorPalette()
{
    delete ui;
}


void changeColorPalette::on_ok_button_clicked()
{
    emit change_color(ui->colorSlide->value());
    this->close();
}


void changeColorPalette::on_close_button_clicked()
{
    this->close();
}


void changeColorPalette::on_colorSlide_sliderMoved(int position)
{
    emit change_color(position);
}

