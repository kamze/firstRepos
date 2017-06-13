#ifndef FACE_CLIENT_H
#define FACE_CLIENT_H

#include <raspicam/raspicam_cv.h>
#include <QPixmap>
#include <QImage>
#include <QTimer>

#include <QDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QTcpSocket>
#include <QTcpSocket>
#include <QDataStream>


QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

//! [0]
class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = Q_NULLPTR);

private slots:
    void requestNewFortune();
    //void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetFortuneButton();
    void sessionOpened();

    void decode();

private:
    void decompressImage();
    QLabel *imageLbl;


    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QLabel *statusLabel1;
    QLabel *timeServer_Label;
    QLabel *timeClient_Label;
    QPushButton *getFortuneButton;

    QLabel *pression_Label;
    QLabel *temperature_Label;
    QLabel *altitude_Label;

    QTcpSocket *tcpSocket;
    QString currentFortune;

    QNetworkSession *networkSession;

    qint32 blocksize;
    QImage MatToQimage(cv::Mat img);
};
//! [0]

#endif
