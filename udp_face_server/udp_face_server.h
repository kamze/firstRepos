#ifndef FACE_SERVER_H
#define FACE_SERVER_H

#include <QDialog>
#include <raspicam/raspicam_cv.h>
// les 3 pour CascadeClassifier
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QtNetwork>
#include <QUdpSocket>


class QLabel;
class QPushButton;
// **class QTcpServer;
class QNetworkSession;

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = Q_NULLPTR);
    raspicam::RaspiCam_Cv Camera;
    std::vector<uchar> compressed_data;
    cv::CascadeClassifier faceClassifier;
    cv::Mat flip_image;


private slots:
    void sessionOpened();
    void sendPacket();


private:
    void setupNewConnection();
    void takePicture();
    void faceDetection();
    void imageCompression();
    void affichageVideo();
    QImage MatToQimage(cv::Mat inMat);


    QLabel *statusLabel;
    QLabel *imageLbl;
    QUdpSocket *udpSocket;
    QStringList fortunes;
    QNetworkSession *networkSession;
    std::vector<QUdpSocket*> clientsConnectees;

};

#endif
