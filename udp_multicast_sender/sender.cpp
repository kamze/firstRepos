#include <QtWidgets>
#include <QtNetwork>
#include <QTime>
#include "RTIMULib.h"
#include "sender.h"

Sender::Sender(QWidget *parent)
    : QDialog(parent)
    , imageLbl(new QLabel)
{
    //------ sensors--------------
    settings = new RTIMUSettings("RTIMULib");
    imu = RTIMU::createIMU(settings);
    pressure = RTPressure::createPressure(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }
    //  This is an opportunity to manually override any settings before the call IMUInit
    //  set up IMU
    qDebug()<<"Init sense hat";
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
    qDebug()<<"Init classifier";
    //------ camera--------------
    if(!faceClassifier.load("/home/pi/TP_SY22/haarcascade_frontalface_alt.xml")){exit(-1);}
//use frameTimer to controle frame frequency
    QTimer *frameTimer =  new QTimer(this);
    connect(frameTimer,&QTimer::timeout,this,&Sender::affichageVideo);
    frameTimer->setInterval(1);
    frameTimer->start();

    Camera.set( CV_CAP_PROP_FRAME_WIDTH, 256 );
    Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 144 );

//setup the image type : here is colorful we should change CV_8UC3 to block and white CV_8UC1 is gray
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
    Camera.open();
    groupAddress = QHostAddress("239.255.43.21");

    statusLabel = new QLabel(tr("Ready to multicast datagrams to group %1 on port 45454").arg(groupAddress.toString()));

    ttlLabel = new QLabel(tr("TTL for multicast datagrams:"));
    ttlSpinBox = new QSpinBox;
    ttlSpinBox->setRange(0, 255);

    QHBoxLayout *ttlLayout = new QHBoxLayout;
    ttlLayout->addWidget(ttlLabel);
    ttlLayout->addWidget(ttlSpinBox);

    startButton = new QPushButton(tr("&Start"));
    quitButton = new QPushButton(tr("&Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    messageNo = 1;

    connect(ttlSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ttlChanged(int)));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
   // connect(timer, SIGNAL(timeout()), this, SLOT(sendDatagram()));
    connect(timer, SIGNAL(timeout()), this, SLOT(packetGeneration()));

    connect(startButton, SIGNAL(clicked()), this, SLOT(startSending()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(ttlLayout);
    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(imageLbl);

    setLayout(mainLayout);

    setWindowTitle(tr("Multicast Sender"));
    ttlSpinBox->setValue(1);
}

void Sender::ttlChanged(int newTtl)
{
    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, newTtl);
}

void Sender::startSending()
{
    startButton->setEnabled(false);
    timer->start(1);
}

void Sender::sendDatagram()
{
    statusLabel->setText(tr("Now sending datagram %1").arg(messageNo));
QByteArray datagram;
//-------send sesnors data---------
set_up_sensors_values();
datagram.append( pression);
//datagram.append( humidity);
//datagram.append( temperature);

//-------send server time---------

QTime time = QTime::currentTime();
QString format= "hh:mm:ss.zzz";
QString timeServer = time.toString(format);
qDebug() << "time server : " << timeServer;


datagram.append( timeServer);
//------------img------------------
imageCompression();
quint32 imgsize=compressed_data.size();
datagram.append( imgsize);

    for(int i=0;i < compressed_data.size();i++){
        datagram.append( compressed_data[i]);
}
    qDebug() << "datagram :Size avaiable : " << datagram.size();

    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             groupAddress, 45454);
    ++messageNo;
}

void Sender::packetGeneration(){
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

// ----------------Sensors-------------------
    set_up_sensors_values();

    out << pression;
    out << humidity;
    out << temperature;
//    qDebug() << "Size avaiable after sensors : " << datagram.size();

// ----------------Time-------------------
    QTime time = QTime::currentTime();
    QString format= "hh:mm:ss.zzz";
    QString timeString = time.toString(format);
    qDebug() << "time : " << timeString;
    out << timeString;

// ----------------Img-------------------

//    qDebug() << "Size avaiable after time : " << datagram.size();
    imageCompression();
    out << (quint32) compressed_data.size();
//    qDebug() << "Size avaiable after size img: " << datagram.size();

    for(int i=0;i < compressed_data.size();i++){
        out << compressed_data[i];
    }
//    qDebug() << "Size avaiable  after img: " << datagram.size();

    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             groupAddress, 45454);

    qDebug() << "Size avaiable end : " << datagram.size();

}


//fonction pour avoir l'image
void Sender::takePicture()
{
    cv::Mat image;
    //Start capture
    Camera.grab();
    Camera.retrieve ( image);

    //std::cout<<"Stop camera..."<<std::endl;
    cv::flip(image,flip_image,0);
}

void Sender::imageCompression()
{
    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(20);
    cv::imencode(".jpg",flip_image,compressed_data,params);
}
void Sender::affichageVideo()
{
    takePicture();
    faceDetection();
    imageLbl->setPixmap(QPixmap::fromImage(MatToQimage(flip_image)));
    //set_up_sensors_values();

}

//fonction detection visage et draw square on face
void Sender::faceDetection()
{
    cv::Mat grey;
    std::vector<cv::Rect> faces;
// transforme l'image en gray
    cv::cvtColor(flip_image,grey,CV_BGR2GRAY);
// on use gray car c'st plus simple de tecté gray que colour
    faceClassifier.detectMultiScale(grey,faces);

    for(int i=0; i < faces.size();i++){
        cv::rectangle(flip_image,faces[i],cv::Scalar(255,0,0));
    }
}

QImage Sender::MatToQimage(cv::Mat inMat){
    QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

    return image;

}

void Sender::set_up_sensors_values(){
    //  poll at the rate recommended by the IMU

    while (imu->IMURead()) {
        RTIMU_DATA imuData = imu->getIMUData();



        if (pressure != NULL) {
            pressure->pressureRead(imuData);
            pression= imuData.pressure;
            // 130.01 pour le calibration de capteur
            humidity= RTMath::convertPressureToHeight(imuData.pressure)+130.01;
            temperature= imuData.temperature;
            qDebug() << "pressure : " << pression;
            qDebug() << "height above sea: " <<humidity;
            qDebug() << "temperature : " << temperature;
        }

    }
}

