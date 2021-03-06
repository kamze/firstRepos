#ifndef SENDER_H
#define SENDER_H

#include <QDialog>
#include <QHostAddress>
#include <QDialog>
#include <raspicam/raspicam_cv.h>
// les 3 pour CascadeClassifier
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "RTIMULib.h"
#include <QtNetwork>

class QDialogButtonBox;
class QLabel;
class QPushButton;
class QTimer;
class QUdpSocket;
class QSpinBox;

class Sender : public QDialog
{
    Q_OBJECT
    raspicam::RaspiCam_Cv Camera;
    std::vector<uchar> compressed_data;
    cv::CascadeClassifier faceClassifier;
    cv::Mat flip_image;


public:
    Sender(QWidget *parent = 0);

private slots:
    void ttlChanged(int newTtl);
    void startSending();
    void sendDatagram();
    void packetGeneration();

private:
    void takePicture();
    void faceDetection();
    void imageCompression();
    void affichageVideo();
    QImage MatToQimage(cv::Mat inMat);

//------- variable----------
    QLabel *statusLabel;
    QLabel *ttlLabel;
    QSpinBox *ttlSpinBox;
    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QUdpSocket *udpSocket;
    QTimer *timer;
    QHostAddress groupAddress;
    int messageNo;
    QLabel *imageLbl;

    //----------sensors------
    double pression;
    double temperature;
    double humidity;
    void set_up_sensors_values();

    RTIMUSettings *settings;
    RTIMU *imu ;
    RTPressure *pressure ;
};

#endif
