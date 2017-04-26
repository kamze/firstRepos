#ifndef RECEIVER_H
#define RECEIVER_H

#include <QDialog>
#include <QHostAddress>
#include <raspicam/raspicam_cv.h>
#include <QPixmap>
#include <QImage>
#include <QTimer>


class QLabel;
class QPushButton;
class QUdpSocket;

class Receiver : public QDialog
{
    Q_OBJECT

public:
    Receiver(QWidget *parent = 0);

private slots:
    void processPendingDatagrams();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
    QLabel *imageLbl;
    QImage MatToQimage(cv::Mat img);

};

#endif
