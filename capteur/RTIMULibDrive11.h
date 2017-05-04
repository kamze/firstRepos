#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <QMainWindow>

namespace Ui {
class capteur;
}

class capteur : public QMainWindow
{
    Q_OBJECT

public:
    explicit capteur(QWidget *parent = 0);
    ~capteur();

private:
    Ui::capteur *ui;
};

#endif // CAPTEUR_H
