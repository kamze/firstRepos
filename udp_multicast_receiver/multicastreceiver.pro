QT += network widgets

HEADERS       = receiver.h
SOURCES       = receiver.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/multicastreceiver
INSTALLS += target
LIBS += -L/opt/vc/lib/  -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util
CONFIG += link_pkgconfig
PKGCONFIG += opencv
