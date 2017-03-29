#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTimer *frameTimer =  new QTimer(this);
    QObject::connect(frameTimer,&QTimer::timeout,this,&MainWindow::display_picture);
    frameTimer->setInterval(100);
    frameTimer->start();

    Camera.set( CV_CAP_PROP_FRAME_WIDTH, 640 );
    Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 480 );
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3);

     if (!Camera.open()) {std::cerr<<"Error opening the camera"<<std::endl;return;}
    Camera.open();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    Camera.release();
    delete ui;
}

QImage MainWindow::MatToQimage(cv::Mat inMat){
    QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

    return image;

}


void MainWindow::display_picture()
{

    cv::Mat image;
    //set camera params

    //Open camera
    std::cout<<"Opening Camera..."<<std::endl;

    //Start capture
    Camera.grab();
    Camera.retrieve ( image);

    //std::cout<<"Stop camera..."<<std::endl;

    //save image

    cv::flip(image,flip_image,0);
/*    //Image compression
    std::vector<uchar> compressed_data;
    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(20);
    cv::imencode(".jpg",flip_image,compressed_data,params);


    image = cv::imdecode(compressed_data,-1);*/
    ui->label->setPixmap(QPixmap::fromImage(MatToQimage(flip_image)));

}


void MainWindow::on_takePicture_clicked()
{
    cv::imwrite("/home/pi/TP_SY22/Camera/raspicam_cv_image.jpg",flip_image);
    std::cout<<"Image saved at raspicam_cv_image.jpg"<<std::endl;
}
