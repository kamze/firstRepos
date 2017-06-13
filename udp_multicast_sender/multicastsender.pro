HEADERS       = sender.h
SOURCES       = sender.cpp \
                main.cpp
QT           += network widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/multicastsender
INSTALLS += target

LIBS += -L/opt/vc/lib/  -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lRTIMULib
CONFIG += link_pkgconfig
PKGCONFIG += opencv
