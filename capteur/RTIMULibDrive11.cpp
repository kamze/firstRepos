#include "capteur.h"
#include "ui_capteur.h"

capteur::capteur(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::capteur)
{
    ui->setupUi(this);
}

capteur::~capteur()
{
    delete ui;
}
