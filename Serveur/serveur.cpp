#include "serveur.h"
#include "ui_serveur.h"

Serveur::Serveur(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Serveur)
{
    ui->setupUi(this);
// if the face detection file is not here then exit.
    if(!faceClassifier.load("/home/pi/haarcascade_frontalface_alt.xml")){exit(-1);}
//use frameTimer to controle frame frequency
    QTimer *frameTimer =  new QTimer(this);
    connect(frameTimer,&QTimer::timeout,this,&Serveur::on_takePic_timeout);
    frameTimer->setInterval(100);
    frameTimer->start();

    //Camera.set( CV_CAP_PROP_FRAME_WIDTH, 640 );
    //Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 480 );
    Camera.set( CV_CAP_PROP_FRAME_WIDTH, 256 );
    Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 144 );
//setup the image type : here is colorful we should change CV_8UC3 to block and white
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );

    // if (!Camera.open()) {std::cerr<<"Error opening the camera"<<std::endl;return;}
    Camera.open();

    //clients = new std::vector<QTcpSocket*>();

    tcpServer = new QTcpServer(this);
    tcpServer->listen();
    connect(tcpServer,&QTcpServer::newConnection,this,&Serveur::onConnect);

    ui->statusLbl->setText(tr("Listening on Port : %1").arg(tcpServer->serverPort()));
}

Serveur::~Serveur()
{
    for(int i=0; i < clients.size(); i++){
        clients[i]->disconnectFromHost();
        delete clients[i];
    }
    delete ui;
    delete tcpServer;
}

void Serveur::onConnect(){
    QTcpSocket *clientSock = tcpServer->nextPendingConnection();
    connect(clientSock,&QAbstractSocket::disconnected,clientSock,&QObject::deleteLater);

    clients.push_back(clientSock);
    ui->statusLbl->setText(ui->statusLbl->text() + "\nNew client : " + clientSock->localAddress().toString());
}

void Serveur::on_sendBtn_clicked()
{
    //QByteArray data = (GetHour()).toUtf8();
    QByteArray packet = generatePacket();
    for(int i = 0; i < clients.size(); i++){
        clients[i]->write(packet);
    }
}
// unsed function
QString Serveur::GetHour(){
    struct timeval date;
    gettimeofday(&date,NULL);

    return tr("%1:%2:%3--%4").arg((date.tv_sec % 86400)/3600 + 1).arg((date.tv_sec % 3600) / 60).arg(date.tv_sec % 60).arg(date.tv_usec);
}

void Serveur::on_quitBtn_clicked()
{
    close();
}

// this function is used to send the data of the camera
QByteArray Serveur::generatePacket(){
    struct timeval date;
    gettimeofday(&date,NULL);

    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
// cette ligne correspond à 4 bytes de tailles 32bits
    out << (quint32) (8 + compressed_data.size() + 4);


    out << (quint32)date.tv_sec;
    out << (quint32)date.tv_usec;

    out << (quint32)compressed_data.size();
    for(int i=0;i < compressed_data.size();i++){
        out << compressed_data[i];

    }
    qDebug() << "Size avaiable : " << block.size();

    return block;
}
// 
void Serveur::on_takePic_timeout()
{

    cv::Mat image;
    cv::Mat flip_image;
    //set camera params

    //Open camera
    //std::cout<<"Opening Camera..."<<std::endl;

    //Start capture
    Camera.grab();
    Camera.retrieve ( image);

    //std::cout<<"Stop camera..."<<std::endl;

    //save image
// why: why do u flip the image
    cv::flip(image,flip_image,0);
    //Face Detection
    cv::Mat grey;
    std::vector<cv::Rect> faces;
// transforme l'image en gray
    cv::cvtColor(flip_image,grey,CV_BGR2GRAY);
    //faceClassifier.detectMultiScale(grey,faces,1.1,2,0|CV_HAAR_SCALE_IMAGE,cv::Size(30,30));
// on use gray car c'st plus simple de tecté gray que colour
    faceClassifier.detectMultiScale(grey,faces);

    for(int i=0; i < faces.size();i++){
        cv::rectangle(flip_image,faces[i],cv::Scalar(255,0,0));
    }

    //Image compression

    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(20);
    cv::imencode(".jpg",flip_image,compressed_data,params);

    ui->imageLbl->setPixmap(QPixmap::fromImage(MatToQimage(flip_image)));
    //cv::imwrite("raspicam_cv_image.jpg",flip_image);

    on_sendBtn_clicked();
}

QImage Serveur::MatToQimage(cv::Mat inMat){
    QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

    return image;

}
