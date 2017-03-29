#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <QTimer>

#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    raspicam::RaspiCam_Cv Camera;
    ~MainWindow();
    cv::Mat flip_image;

private slots:
    void display_picture();


    void on_takePicture_clicked();

private:
    Ui::MainWindow *ui;
   QImage MatToQimage(cv::Mat img);

};

#endif // MAINWINDOW_H
