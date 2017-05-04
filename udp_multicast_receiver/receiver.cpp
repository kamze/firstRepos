#include <QtWidgets>
#include <QtNetwork>
#include <QTime>

#include "receiver.h"

Receiver::Receiver(QWidget *parent)
    : QDialog(parent)
{
    groupAddress = QHostAddress("239.255.43.21");

    statusLabel = new QLabel(tr("Listening for multicasted messages"));
    quitButton = new QPushButton(tr("&Quit"));
    imageLbl = new QLabel();

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddress);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(imageLbl);
    setLayout(mainLayout);
    setWindowTitle(tr("Multicast Receiver"));
}

void Receiver::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        statusLabel->setText(tr("Received datagram: \"%1\"")
                             .arg(datagram.size()));

        qDebug() << "datagram.size : " << datagram.size();
        std::vector<uchar> compressed_data;
        quint8 b;
//-------------le temps---------------
        QTime time = QTime::currentTime();
        QString format= "hh:mm:ss.zzz";
        QString timeClient = time.toString(format);
        QString timeServer;

        /*timeServer = datagram.data();
        char k = datagram.at(0);
        char g = datagram.at(1);
        qDebug() << "k : " << k;
        qDebug() << "g : " << g;*/

        for(int i=0;i < 12;i++){
             b= datagram.at(i);
            timeServer.append(b);
}
        qDebug() << "time client : " << timeClient;
        qDebug() << "time server : " << timeServer;

        for(int i=12;i < datagram.size();i++){
             b= datagram.at(i);
            compressed_data.push_back((uchar)b);
}
    cv::Mat image = cv::imdecode(compressed_data,-1);
    imageLbl->setPixmap(QPixmap::fromImage(MatToQimage(image)));
    }

}

QImage Receiver::MatToQimage(cv::Mat inMat){
    QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

    return image;

}
