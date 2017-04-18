#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T16:08:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Serveur
TEMPLATE = app


SOURCES += main.cpp\
        serveur.cpp

HEADERS  += serveur.h

FORMS    += serveur.ui

LIBS += -L/opt/vc/lib/  -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util
CONFIG += link_pkgconfig
PKGCONFIG += opencv
