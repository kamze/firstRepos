QT       += core
QT       += network
QT       -= gui

TARGET = QUdpSocket
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    myudp.cpp

HEADERS += \
    myudp.h
