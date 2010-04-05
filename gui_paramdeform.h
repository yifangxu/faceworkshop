#ifndef GUI_PARAMDEFORM_H
#define GUI_PARAMDEFORM_H

#include <QMainWindow>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <string>
#include <vector>
#include "myimage.h"
using std::string;
using std::vector;
#include "asmmodel.h"

namespace Ui {
    class GUI_ParamDeform;
}

class ThreadWarping : public QThread{
    Q_OBJECT
public:
    ThreadWarping(){ stopped = false; };
    ~ThreadWarping(){};

    void stop(){
        mutex.lock();
        delete warpPtr;
        stopped = true;
        mutex.unlock();
    }

    bool updateWarpPtr(ImgTrans_MLS *wPtr){
        if (!mutex.tryLock())
            return false;
        warpPtr = wPtr;
        mutex.unlock();
        return true;
    }

    void setImage(cv::Mat &m){
        QMutexLocker locker(&mutex);
        oriImg = m;
    }

    cv::Mat getImage(){
        QMutexLocker locker(&mutex);
        return newImg.clone();
    }

protected:
    void run(){
        if (!mutex.tryLock())
            return;
//        mutex.lock();
        if (!stopped){
            warpPtr->calcDelta();
            newImg = warpPtr->genNewImg(oriImg, 1);
            delete warpPtr;
            warpPtr = NULL;
        }
        mutex.unlock();
    }
private:
    cv::Mat oriImg, newImg;
    QMutex mutex;
    bool stopped;
    ImgTrans_MLS *warpPtr;
};

class GUI_ParamDeform : public QMainWindow {
    Q_OBJECT
public:
    GUI_ParamDeform(QWidget *parent = 0);
    ~GUI_ParamDeform();

public slots:
    void updatePic();
    void showWarpRes();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GUI_ParamDeform *ui;

    ASMModel asmModel;

    vector< FitResult > fitResV;
    Mat oriImg;

    vector< cv::Point2i > fittedPointV;

    cv::CascadeClassifier faceCascade;

    bool bDeformPic;

    void loadImg(Mat& img);

    ThreadWarping threadWarp;

private slots:
    void on_tabProcessMethod_currentChanged(int index);
    void on_actionZoom_out_triggered();
    void on_actionZoom_In_triggered();
    void on_actCapFromCam_triggered();
    void on_chkDeform_toggled(bool checked);
    void on_wParamEdit_paramUpdated();
    void on_actionLoadImg_triggered();
};

#endif // GUI_PARAMDEFORM_H
