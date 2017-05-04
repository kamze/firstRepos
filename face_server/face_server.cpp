#include <QtWidgets>
#include <QtNetwork>
#include <QTimer>
#include <QDebug>
#include <QTime>

#include <stdlib.h>

#include "face_server.h"
#include "RTIMULib.h"
#include "RTIMUSettings.h"

Server::Server(QWidget *parent)
    : QDialog(parent)
    , statusLabel(new QLabel)
    , imageLbl(new QLabel)
    , tcpServer(Q_NULLPTR)
    , networkSession(0)
{
    //-----------sensor--------------
        settings = new RTIMUSettings("RTIMULib");
        imu = RTIMU::createIMU(settings);
        pressure = RTPressure::createPressure(settings);

        if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
            printf("No IMU found\n");
            exit(1);
        }
        //  This is an opportunity to manually override any settings before the call IMUInit
        //  set up IMU
        imu->IMUInit();
        //  this is a convenient place to change fusion parameters
        imu->setSlerpPower(0.02);
        imu->setGyroEnable(true);
        imu->setAccelEnable(true);
        imu->setCompassEnable(true);
        //  set up pressure sensor
        if (pressure != NULL)
            pressure->pressureInit();
        //  set up for rate timer

        //-------------------camera setup------------------------
    if(!faceClassifier.load("/home/pi/TP_SY22/haarcascade_frontalface_alt.xml")){exit(-1);}
//use frameTimer to controle frame frequency
    QTimer *frameTimer =  new QTimer(this);
    connect(frameTimer,&QTimer::timeout,this,&Server::sendPacket);
    frameTimer->setInterval(1);
    frameTimer->start();

    Camera.set( CV_CAP_PROP_FRAME_WIDTH, 256 );
    Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 144 );

//setup the image type : here is colorful we should change CV_8UC3 to block and white CV_8UC1 is gray
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
    Camera.open();
//---------------------camera ends here------------------------

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);


    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);


        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }



        QPushButton *quitButton = new QPushButton(tr("Quit"));
        quitButton->setAutoDefault(false);
        connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
        // pour envoyer le button on le use plus car on veut automatoqiement envoyer des  image  successive
        connect(tcpServer,&QTcpServer::newConnection,this,&Server::setupNewConnection);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(quitButton);
        buttonLayout->addStretch(1);


        QVBoxLayout *mainLayout = Q_NULLPTR;
        if (QGuiApplication::styleHints()->showIsFullScreen()) {
            QVBoxLayout *outerVerticalLayout = new QVBoxLayout(this);
            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
            QHBoxLayout *outerHorizontalLayout = new QHBoxLayout;
            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
            QGroupBox *groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
            mainLayout = new QVBoxLayout(groupBox);
            outerHorizontalLayout->addWidget(groupBox);
            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
            outerVerticalLayout->addLayout(outerHorizontalLayout);
            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        } else {
            mainLayout = new QVBoxLayout(this);
        }

        mainLayout->addWidget(statusLabel);
        mainLayout->addWidget(imageLbl);
        mainLayout->addLayout(buttonLayout);

        setWindowTitle(QGuiApplication::applicationDisplayName());
}


//-----------------------END constructor---------------------------------------
//fonction pour envoyé

//-----------------------sensor----------------------
void Server::set_up_sensors_values(){
    //  poll at the rate recommended by the IMU

    while (imu->IMURead()) {
        RTIMU_DATA imuData = imu->getIMUData();
        now = RTMath::currentUSecsSinceEpoch();

            if (pressure != NULL) {
                pressure->pressureRead(imuData);
            /*    qDebug() << "pressure : " << imuData.pressure;
                qDebug() << "height above sea: " << RTMath::convertPressureToHeight(imuData.pressure);
                qDebug() << "temperature : " << imuData.temperature;*/

}

    }
}

void Server::faceDetection(){
    cv::Mat grey;
    std::vector<cv::Rect> faces;
// transforme l'image en gray
    cv::cvtColor(flip_image,grey,CV_BGR2GRAY);
// on use gray car c'st plus simple de tecté gray que colour
    faceClassifier.detectMultiScale(grey,faces);

    //fonction detection visage et draw square on face
    for(int i=0; i < faces.size();i++){
        cv::rectangle(flip_image,faces[i],cv::Scalar(255,0,0));
    }
}


void Server::takePicture(){
    cv::Mat image;
    //Start capture
    Camera.grab();
    Camera.retrieve ( image);

    //std::cout<<"Stop camera..."<<std::endl;
    cv::flip(image,flip_image,0);
}

void Server::sendPacket(){
takePicture();
faceDetection();
imageCompression();
affichageVideo();
packetGeneration();
}

QByteArray Server::packetGeneration(){
    QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);

        QTime time = QTime::currentTime();
        QString format= "hh:mm:ss.zzz";
        QString timeString = time.toString(format);
        qDebug() << "time : " << timeString;
        qDebug() << "time : " << timeString;

        out << timeString;

        out << (quint32) compressed_data.size();



        for(int i=0;i < compressed_data.size();i++){
            out << compressed_data[i];
}
        for(int i = 0; i < clientsConnectees.size(); i++){

            clientsConnectees[i]->write(block);
       }


        qDebug() << "Size avaiable : " << block.size();

    return block;
}

QImage Server::MatToQimage(cv::Mat inMat){
    QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

    return image;

}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));

}

void Server::setupNewConnection(){

    QTcpSocket *clientSock = tcpServer->nextPendingConnection();
    //connect(clientSock,&QAbstractSocket::disconnected,clientSock,&QObject::deleteLater);

    clientsConnectees.push_back(clientSock);
}
void Server::imageCompression()
{
    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(20);
    cv::imencode(".jpg",flip_image,compressed_data,params);
}
void Server::affichageVideo()
{
    imageLbl->setPixmap(QPixmap::fromImage(MatToQimage(flip_image)));
    set_up_sensors_values();
}

