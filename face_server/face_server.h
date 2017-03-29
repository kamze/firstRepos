#ifndef FACE_SERVER_H
#define FACE_SERVER_H

#include <QDialog>
#include <raspicam/raspicam_cv.h>
// les 3 pour CascadeClassifier
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>




class QLabel;
class QPushButton;
class QTcpServer;
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
    //void sendFortune();
    void sendPacket();


private:
    void takePicture();
    void faceDetection();
    void imageCompression();
    QByteArray packetGeneration();
    void affichageVideo();
    QImage MatToQimage(cv::Mat inMat);


    QLabel *statusLabel;
    QLabel *imageLbl;
    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;
};

#endif
