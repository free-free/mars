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
    spsettingsdialog.cpp \
    qcustomplot.cpp \
    marsbyteslistbuffer.cpp \
    marsconsole.cpp \
    marsserialport.cpp \
    marscommandline.cpp \
    marsfigure.cpp \
    marsplot.cpp \
    marsbytesqueue.cpp \
    helper.cpp \
    mars.cpp

HEADERS  += \
    spsettingsdialog.h \
    marstypes.h \
    qcustomplot.h \
    marsbyteslistbuffer.h \
    marsconsole.h \
    marscommandline.h \
    marsserialport.h \
    marsfigure.h \
    marsplot.h \
    marsbytesqueue.h \
    helper.h \
    mars.h

FORMS    += \
    spsettingsdialog.ui \
    mars.ui

RESOURCES += \
    mars_resource.qrc

RC_FILE += application.rc
