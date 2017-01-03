#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T16:44:53
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mars
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    spsettingsdialog.cpp \
    qcustomplot.cpp \
    marsbyteslistbuffer.cpp \
    marsconsole.cpp \
    marsserialport.cpp \
    marscommandline.cpp \
    marsdatawidget.cpp \
    marsfigure.cpp

HEADERS  += mainwindow.h \
    spsettingsdialog.h \
    marstypes.h \
    qcustomplot.h \
    marsbyteslistbuffer.h \
    marsconsole.h \
    marscommandline.h \
    marsserialport.h \
    marsdatawidget.h \
    marsfigure.h

FORMS    += mainwindow.ui \
    spsettingsdialog.ui

RESOURCES += \
    mars_resource.qrc
