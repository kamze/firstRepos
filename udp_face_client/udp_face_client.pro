QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = udp_face_client
TEMPLATE = app


SOURCES += main.cpp\
        udp_face_client.cpp

HEADERS  += udp_face_client.h

FORMS    += udp_face_client.ui

LIBS += -L/opt/vc/lib/  -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util
CONFIG += link_pkgconfig
PKGCONFIG += opencv
