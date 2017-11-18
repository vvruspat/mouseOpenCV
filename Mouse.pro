#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T22:02:22
#
#-------------------------------------------------
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mouse
TEMPLATE = app

INCLUDEPATH += /usr/local/opt/opencv@3/include
LIBS += -L/usr/local/opt/opencv@3/lib -lopencv_core -lopencv_imgproc -lopencv_tracking -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
