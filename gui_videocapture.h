#ifndef GUI_VIDEOCAPTURE_H
#define GUI_VIDEOCAPTURE_H

#include <QDialog>
#include <QGraphicsScene>
#include <QThread>
#include <QMutex>

#include "highgui.h"
using cv::VideoCapture;

namespace Ui {
    class GUI_VideoCapture;
}

//! Asynchrously capturing images from a webcam.
class ImgCaptureThread : public QThread {
    Q_OBJECT
public:
    ImgCaptureThread(){ stopped = false; }
    ~ImgCaptureThread(){}

    cv::Mat & image(){ return img;}
    VideoCapture *capture;

    void stop(){
        mutex.lock();
        capture->release();
        stopped = true;
        mutex.unlock();
    }

protected:
    void run(){
        if (!mutex.tryLock())
            return;
        if (!stopped){
            cv::Mat imgT;
            (*capture)>>imgT;
            cv::flip(imgT, img, 1);
        }
        mutex.unlock();
    }
private:
    cv::Mat img;
    QMutex mutex;
    bool stopped;
};

//! UI for capturing an image from webcam
class GUI_VideoCapture : public QDialog {
    Q_OBJECT
public:
    GUI_VideoCapture(QWidget *parent = 0);
    ~GUI_VideoCapture();

    void stopCapture();
    void startCapture();

    QGraphicsScene gScene;

    ImgCaptureThread thread;
    cv::Mat img;
public slots:
    void newImageReady();
protected:
    virtual void changeEvent( QEvent *e );
    virtual void timerEvent( QTimerEvent *event);
    virtual void hideEvent( QHideEvent *e );
private:
    VideoCapture capture;

    int timerId;

private:
    Ui::GUI_VideoCapture *ui;

private slots:
    void on_cmbCamSelect_currentIndexChanged(int index);
};

#endif // GUI_VIDEOCAPTURE_H
