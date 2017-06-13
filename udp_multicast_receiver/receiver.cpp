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
    timeServer_Label = new QLabel();
    timeClient_Label = new QLabel();

    // initailisation des label pour les valeurs des capteur
    pression_Label = new QLabel();
    temperature_Label = new QLabel();
    altitude_Label = new QLabel();

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddress);

    /*connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));*/
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(decode()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(timeServer_Label);
    mainLayout->addWidget(timeClient_Label);

    mainLayout->addWidget(pression_Label);
    mainLayout->addWidget(temperature_Label);
    mainLayout->addWidget(altitude_Label);


    mainLayout->addWidget(imageLbl);
    mainLayout->addLayout(buttonLayout);

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

//-------------sensors read---------------
        QByteArray pression;
        // pression byte  array
        //double humidity;
       // double temperature;

        for(int i=0;i < sizeof(double);i++){
            pression.append(datagram.at(i));
}

        double ppression = pression.toDouble();
        qDebug() << "ppression: " << ppression;

//-------------le temps---------------
        QTime time = QTime::currentTime();
        QString format= "hh:mm:ss.zzz";
        QString timeClient_display = "Client: " ;
        QString timeServer;
        QString timeServer_display;

        timeClient_display.append(time.toString(format));


//        for(int i=4;i < 17;i++){
          for(int i=2;i < 15;i++){
             b= datagram.at(i);
            timeServer.append(b);
}
        timeServer_display= "Server: ";
        timeServer_display.append(timeServer);
        qDebug() << timeClient_display;
        qDebug() << timeServer_display;

        timeServer_Label->setText(timeServer_display);
        timeClient_Label->setText(timeClient_display);


//---------image---------------
//       for(int i=17;i < datagram.size();i++){
          for(int i=15;i < datagram.size();i++){
             b= datagram.at(i);
            compressed_data.push_back((uchar)b);
}
    cv::Mat image = cv::imdecode(compressed_data,-1);
    imageLbl->setPixmap(QPixmap::fromImage(MatToQimage(image)));
    }

}


void Receiver::decode(){
    QByteArray datagram;
    qDebug() << "udp socket size : " << udpSocket->size();

   do {
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
    } while (udpSocket->hasPendingDatagrams());
// apres avoir le datagramme on lis avec in comme avec le client tcp
  QDataStream in( &datagram, QIODevice::ReadOnly);
  in.setVersion( QDataStream::Qt_4_0 );

    std::vector<uchar> compressed_data;
    quint8 b;
    quint32 imgSize;

    //  --------------------sensor-----------------
    double pression;
    double altitude;
    double temperature;
    // lire les valeur des capteurs envoyé par le serveur
    in >> pression;
    in >> altitude;
    in >> temperature;
    // faire un string pour afficher sur le label les données
    QString phrase_pression= "pression : ";
    phrase_pression.append(QString::number(pression));
    QString phrase_altitude= "height above sea: ";
    phrase_altitude.append(QString::number(altitude));
    QString phrase_temperature= "temperature : " ;
    phrase_temperature.append(QString::number(temperature));
    // affichier les valeur sur les label
    pression_Label->setText(phrase_pression);
    temperature_Label->setText(phrase_temperature);
    altitude_Label->setText(phrase_altitude);
    // pour debuger
    qDebug() << "pressure : " << pression;
    qDebug() << "height above sea: " <<altitude;
    qDebug() << "temperature : " << temperature;

    //  --------------------time-----------------
    QTime time = QTime::currentTime();
    QString format= "hh:mm:ss.zzz";
    QString timeClient_display = "Client: " ;
    QString timeServer;
    QString timeServer_display;

    timeClient_display.append(time.toString(format));
    in >> timeServer;
    timeServer_display= "Server: ";
    timeServer_display.append(timeServer);

    qDebug() << timeClient_display;
    qDebug() << timeServer_display;

    timeServer_Label->setText(timeServer_display);
    timeClient_Label->setText(timeClient_display);
//--------------size & img---------------
        in >> imgSize;
        qDebug() << "imgSize : " << imgSize;

        for(int i=0;i < imgSize;i++){
            in >> b;
            compressed_data.push_back((uchar)b);
}

    cv::Mat image = cv::imdecode(compressed_data,-1);


    imageLbl->setPixmap(QPixmap::fromImage(MatToQimage(image)));

    qDebug() << "----------------------------";

}

QImage Receiver::MatToQimage(cv::Mat inMat){
    QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

    return image;

}

