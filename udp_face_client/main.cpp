#include <QApplication>
#include "udp_face_client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(Client::tr("Fortune Client"));
    Client client;
    client.show();
    return app.exec();
}
