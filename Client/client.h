#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDataStream>
#include <QDebug>

#include <raspicam/raspicam_cv.h>
#include <QPixmap>
#include <QImage>
#include <QTimer>

#include <vector>
#include <sys/time.h>

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void on_quitBtn_clicked();

    void on_connectBtn_clicked();

    void onReceived();

private:
    Ui::Client *ui;

    QTcpSocket* serverSocket;
    QDataStream in;

    qint32 blocksize;

    QString GetHour(struct timeval date);
    QImage MatToQimage(cv::Mat img);
};

#endif // CLIENT_H
