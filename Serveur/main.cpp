#include "serveur.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Serveur w;
    w.show();

    return a.exec();
}
