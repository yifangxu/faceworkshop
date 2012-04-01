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
        if (!capture.open(i))
            break;
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
    thread.capture = &capture;
    connect(&thread, SIGNAL(finished()), this, SLOT(newImageReady()));
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
    if (capture.isOpened()){
        ui->graphicsView->setMinimumSize(
                int(capture.get(CV_CAP_PROP_FRAME_WIDTH)*1.1),
                int(capture.get(CV_CAP_PROP_FRAME_HEIGHT)*1.1));
        timerId = startTimer(100);
    }
}

void GUI_VideoCapture::stopCapture(){
    killTimer( timerId );
    thread.stop();
}

void GUI_VideoCapture::timerEvent(QTimerEvent *event){
    thread.start();
}

void GUI_VideoCapture::newImageReady()
{
    img = thread.image();
    gScene.clear();
    gScene.addPixmap(QPixmap::fromImage(MyImage::fromMat(img)));
    gScene.update();
    this->update();
}

void GUI_VideoCapture::on_cmbCamSelect_currentIndexChanged(int index)
{
    if (capture.isOpened())
        capture.release();
    qDebug("-- Testing camera %d...", index);
    capture.open(index);
    startCapture();
}
