#-------------------------------------------------
#
# Project created by QtCreator 2013-11-06T22:39:00
#
#-------------------------------------------------

QT       += core gui network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = heatmap
TEMPLATE = app


SOURCES += main.cpp \
    heatmap.cpp \
    udpadapter.cpp \
    mywebview.cpp \
    mainwindow.cpp

HEADERS  += \
    heatmap.h \
    udpadapter.h \
    mywebview.h \
    macros.h \
    mainwindow.h

FORMS    +=

RESOURCES += \
    jquery.qrc
