#ifndef SERVEUR_H
#define SERVEUR_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QDataStream>
#include <QString>
#include <raspicam/raspicam_cv.h>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <QDebug>

#include <vector>
#include <sys/time.h>
#include <iostream>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class Serveur;
}

class Serveur : public QMainWindow
{
    Q_OBJECT

public:
    explicit Serveur(QWidget *parent = 0);
    ~Serveur();

    QString GetHour();
    raspicam::RaspiCam_Cv Camera;
    std::vector<uchar> compressed_data;
    cv::CascadeClassifier faceClassifier;


private slots:
    void onConnect();

    void on_sendBtn_clicked();

    void on_quitBtn_clicked();
    void on_takePic_timeout();

private:
    Ui::Serveur *ui;

    QTcpServer *tcpServer;

    std::vector<QTcpSocket*> clients;

   // QByteArray generatePacket(QByteArray data);
    QByteArray generatePacket(struct timeval data);
    QByteArray generatePacket();

    QImage MatToQimage(cv::Mat img);

};

#endif // SERVEUR_H
