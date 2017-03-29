#-------------------------------------------------
#
# Project created by QtCreator 2017-03-29T00:24:24
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_client
TEMPLATE = app


SOURCES += main.cpp\
        face_client.cpp

HEADERS  += face_client.h

FORMS    += face_client.ui

LIBS += -L/opt/vc/lib/  -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util
CONFIG += link_pkgconfig
PKGCONFIG += opencv
