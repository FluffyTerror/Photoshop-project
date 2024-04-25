QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    color_palette.cpp \
    cut_image_mod.cpp \
    events.cpp \
    huesaturation.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    color_palette.h \
    cut_image_mod.h \
    events.h \
    mainwindow.h \
    huesaturation.h \
    type_function.h


FORMS += \
    color_palette.ui \
    cut_image_mod.ui \
    huesaturation.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
