#-------------------------------------------------
#
# Project created by QtCreator 2015-06-16T17:13:13
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ColorSlider
TEMPLATE = app

INCLUDEPATH += C:/Qt/extras/qcustomplot/


SOURCES += main.cpp\
    colorslider.cpp \
    clickableqlabel.cpp \
    C:/Qt/extras/qcustomplot/qcustomplot.cpp \
    dataprocessing.cpp \
    pastraffle.cpp

HEADERS  += colorslider.h \
    C:/Qt/extras/qcustomplot/qcustomplot.h \
    clickableqlabel.h \
    layoutoverlay.h \
    dataprocessing.h \
    pastraffle.h

FORMS    += colorslider.ui

CONFIG += c++11

#RESOURCES += \
#    resource.qrc

DISTFILES += \
    Enhancements.txt \
    History.md

RESOURCES += \
    images.qrc
