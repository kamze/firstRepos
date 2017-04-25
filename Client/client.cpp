#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client),
    serverSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(serverSocket,&QTcpSocket::readyRead,this,&Client::onReceived);
    blocksize = 0;

    in.setDevice(serverSocket);
    in.setVersion(QDataStream::Qt_4_0);
}

Client::~Client()
{
    delete ui;
}

void Client::on_quitBtn_clicked()
{
    close();
}

void Client::on_connectBtn_clicked()
{

    serverSocket->abort();
    serverSocket->connectToHost(ui->serverNameTBox->toPlainText(),ui->serverPortTBox->toPlainText().toInt());

    ui->statusLbl->setText("Connected");
}

QString Client::GetHour(struct timeval date){
    return tr("%1:%2:%3--%4").arg((date.tv_sec % 86400)/3600 + 1).arg((date.tv_sec % 3600) / 60).arg(date.tv_sec % 60).arg(date.tv_usec);
}
//--------- RECEIVING THE PACKET---------------
void Client::onReceived(){
    QDataStream in(serverSocket);
    in.setVersion(QDataStream::Qt_4_0);
    qDebug() << "bytesAvailable : " << serverSocket->bytesAvailable();

    if(blocksize == 0){
        if(serverSocket->bytesAvailable() < sizeof(qint32)){
            return;
        }
        // ON LIS LES 4 PREMIER BYTE
        in >> blocksize;
        qDebug() << "Size avaiable : " << serverSocket->bytesAvailable();
    }

    if(serverSocket->bytesAvailable() < blocksize){
        return;
    }

    //QString data;
    //in >> data;
    struct timeval date;
    //in.readRawData((char*)&date,sizeof(struct timeval));
    //in.skipRawData(blocksize);

    in >> (quint32&)date.tv_sec;
    in >> (quint32&)date.tv_usec;

    std::vector<uchar> compressed_data;
    quint32 imgSize;
    in >> imgSize;
    quint8 b;
    for(int i=0;i < imgSize;i++){
        in >> b;
        compressed_data.push_back((uchar)b);
    }

    QString heure = GetHour(date);
    gettimeofday(&date,NULL);
    heure += "\n" + GetHour(date) ;
    cv::Mat image = cv::imdecode(compressed_data,-1);
    qDebug() << "afterreading vbyteavailable: " << serverSocket->bytesAvailable();

    ui->imageLbl->setPixmap(QPixmap::fromImage(MatToQimage(image)));
    ui->statusLbl->setText(heure);

    blocksize = 0;

}

QImage Client::MatToQimage(cv::Mat inMat){
    QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

    return image;

}
