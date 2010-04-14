#include "widget_facephotoview.h"
#include "gui_videocapture.h"
#include <QFileDialog>
#include "myimage.h"

#include <QTime>

Widget_FacePhotoView::Widget_FacePhotoView(QWidget *parent) :
    PhotoView(parent)
{
}

void Widget_FacePhotoView::modelFit()
{
    QTime timeObj;
    timeObj.start();
    asmModel->fit(oriImg, fitResV, *faceCascade, true, 0);
    qDebug("ASM Time: %d", timeObj.elapsed());
//    asmModel.showResult(img, fitResult);

    vector< cv::Point2i > fittedPointV;

    MyImage mImg=MyImage::fromMat(oriImg);
    setImage(mImg);
    if (fitResV.size()>0){
        asmModel->resultToPointList(fitResV[0], fittedPointV);
        setOriPointList(getQListQPoint(fittedPointV));
//        Mat_<double> m2=asmModel->normalizeParam(fitResV[0].params);
//        ui->wParamEdit->setParamV(m2);
    }
}

void Widget_FacePhotoView::loadImgFromFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
               tr("Open List file"), "", tr("Image Files (*.jpg *.png *.ppm);;All Files (*.*)"));

    Mat img;
    img = cv::imread(fileName.toLocal8Bit().data());

    if (!img.empty()){
        oriImg = img;
        modelFit();
    }
}

void Widget_FacePhotoView::loadImgFromCam()
{
    GUI_VideoCapture gvc(this);
    gvc.exec();

    if (!gvc.img.empty()){
        oriImg = gvc.img;
        modelFit();
    }
}

void Widget_FacePhotoView::loadAllFromPTS()
{
}
