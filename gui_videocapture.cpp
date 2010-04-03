#include "gui_videocapture.h"
#include "ui_gui_videocapture.h"

#include "myimage.h"

GUI_VideoCapture::GUI_VideoCapture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GUI_VideoCapture)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&gScene);
    int i=0;
    while (i<10){
        capture.open(i);
        if (!capture.isOpened())
            break;
        QString s;
        s.sprintf("%d: %dx%d", i,
            int(capture.get(CV_CAP_PROP_FRAME_WIDTH)),
            int(capture.get(CV_CAP_PROP_FRAME_HEIGHT)));
        ui->cmbCamSelect->addItem(s);
        capture.release();
        i++;
    }
    capture.open(0);
    startCapture();
}

GUI_VideoCapture::~GUI_VideoCapture()
{
    delete ui;
}

void GUI_VideoCapture::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GUI_VideoCapture::hideEvent( QHideEvent *e ){
    stopCapture();
    QDialog::hideEvent(e);
}

void GUI_VideoCapture::startCapture(){
//    capture.open(0);
//    capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
//    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 800);
    if (capture.isOpened()){
        ui->graphicsView->setMinimumSize(
                int(capture.get(CV_CAP_PROP_FRAME_WIDTH)*1.1),
                int(capture.get(CV_CAP_PROP_FRAME_HEIGHT)*1.1));
        timerId = startTimer(25);
    }
}

void GUI_VideoCapture::stopCapture(){
    killTimer( timerId );
    capture.release();

    // cvReleaseImage(&frame_copy);
}

void GUI_VideoCapture::timerEvent(QTimerEvent *event){
//    qDebug("HEEERRE");
    cv::Mat imgT;
    capture>>imgT;
    cv::flip(imgT, img, 1);
    gScene.clear();
    gScene.addPixmap(QPixmap::fromImage(MyImage::fromMat(img)));
    gScene.update();
    /*
    cvReleaseImage(&frame_copy);
    cvReleaseImage(&pCannyGray);
    cvReleaseImage(&gray);*/
    //cvReleaseImage(&frame);
}

void GUI_VideoCapture::on_cmbCamSelect_currentIndexChanged(int index)
{
    if (capture.isOpened())
        capture.release();
    qDebug("--Index: %d", index);
    capture.open(index);
    startCapture();
}
