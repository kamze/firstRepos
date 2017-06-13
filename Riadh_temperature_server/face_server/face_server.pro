#-------------------------------------------------
#
# Project created by QtCreator 2017-03-29T00:23:26
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_server
TEMPLATE = app


SOURCES += main.cpp\
        face_server.cpp

HEADERS  += face_server.h

FORMS    += face_server.ui

LIBS += -L/opt/vc/lib/  -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lRTIMULib
CONFIG += link_pkgconfig
PKGCONFIG += opencv
#LDFLAGS += -lRTIMULib

