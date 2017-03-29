#include "face_server.h"
#include <QApplication>
#include <QtCore>

#include <stdlib.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(Server::tr("Fortune Server"));
    Server server;
    server.show();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return app.exec();
}
