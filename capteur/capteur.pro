#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T00:14:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = capteur
TEMPLATE = app


SOURCES += main.cpp \
    RTIMULibDrive11.cpp

HEADERS  += \
    RTIMULibDrive11.h

FORMS    += capteur.ui

LDFLAGS += -lRTIMULib

#RTIMULibDrive11 : RTIMULibDrive11.cpp

#clean:
#	rm RTIMULibDrive11
