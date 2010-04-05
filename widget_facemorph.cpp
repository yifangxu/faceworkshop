#include "widget_facemorph.h"
#include "ui_widget_facemorph.h"
#include "myimage.h"
#include "gui_videocapture.h"
#include "imgtranspiecewiseaffine.h"
#include "qfiledialog.h"
#include "util.h"

Widget_FaceMorph::Widget_FaceMorph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget_FaceMorph)
{
    string appPath = QApplication::applicationDirPath().toStdString();
    ui->setupUi(this);
//     asmModel.readTrainData((appRoot+"data/feret_shape/pts.list").c_str());
    string modelPath;
    //modelPath = "/home/chenxing/myProj/aamlib-opencv/data/color_asm68.model";
    modelPath = appPath + "/data/color_asm75_ascii.model";
    asmModel.load(modelPath);

    modelPath = appPath + "/data/color_sam68.model";
    //samModel.load(modelPath);

    string faceCascadePath=QApplication::applicationDirPath().toStdString() + "/data/haarcascade_frontalface_alt.xml";
    faceCascade.load(faceCascadePath);

    ui->view1->pointPaint.setShapeInfo(&asmModel.getShapeInfo());
    ui->view2->pointPaint.setShapeInfo(&asmModel.getShapeInfo());
}

Widget_FaceMorph::~Widget_FaceMorph()
{
    delete ui;
}

void Widget_FaceMorph::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget_FaceMorph::loadImg(Mat& imgA, int i)
{
    img[i] = imgA;
    asmModel.fit(img[i], fitResV[i], faceCascade, true, 0);
//    asmModel.showResult(img, fitResult);

    MyImage mImg=MyImage::fromMat(img[i]);
    if (i==0)
        ui->view1->setImage(mImg);
    else
        ui->view2->setImage(mImg);
    if (fitResV[i].size()>0){
        asmModel.resultToPointList(fitResV[i][0], fittedPointV[i]);
        if (i==0)
            ui->view1->setPointList(getQListQPoint(fittedPointV[i]));
        else
            ui->view2->setPointList(getQListQPoint(fittedPointV[i]));
        //Mat_<double> m2=asmModel.normalizeParam(fitResV[i][0].params);
        //ui->wParamEdit->setParamV(m2);
    }
}

void Widget_FaceMorph::on_btnCapImg1_clicked()
{
    GUI_VideoCapture gvc(this);
    gvc.exec();
    loadImg(gvc.img, 0);
}

void Widget_FaceMorph::on_btnLoadImg2_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
               tr("Open List file"), "./", tr("Image Files (*.jpg *.png *.ppm);;All Files (*.*)"));

    Mat img;
    img = cv::imread(fileName.toStdString());
    loadImg(img, 1);
}

void Widget_FaceMorph::on_btnRender_clicked()
{
//    ImgTrans_MLS_Rigid warpObj, invWarp;
//     ImgTrans_MLS_Similarity warpObj;
//    ImgTransPieceWiseAffine warpObj, invWarp;
//    vector< cv::Point2i > vTar;
//    FitResult fTar;
//
    int vDiv = 100;
    double di;
//    for (int i=0; i<vDiv; i++){
//        di = i/(double)vDiv;
//        fTar.params = this->fitResV[0][0].params * (1-di) + this->fitResV[1][0].params * di;
//        fTar.transformation = this->fitResV[0][0].transformation;
//
//        asmModel.resultToPointList(fTar, vTar);
//
//        warpObj.setTargetSize(img[0].cols, img[0].rows);
//        warpObj.setSize(img[0].cols, img[0].rows);
//        warpObj.setSrcPoints(fittedPointV[0]);
//        warpObj.setDstPoints(vTar);
//        warpObj.alpha = 1;
//        warpObj.gridSize = 4;
//        warpObj.calcDelta();
//
//        invWarp.setTargetSize(img[0].cols, img[0].rows);
//        invWarp.setSize(img[1].cols, img[1].rows);
//        invWarp.setSrcPoints(fittedPointV[1]);
//        invWarp.setDstPoints(vTar);
//        invWarp.alpha = 1;
//        invWarp.gridSize = 4;
//        invWarp.calcDelta();
//
//        Mat mo1 = warpObj.genNewImg(img[0], 1);
//        Mat mo2 = invWarp.genNewImg(img[1], 1);
//        Mat mTmp1, mTmp2, mTmp3;
//        Mat_<uchar> faceRegion;
//        faceRegion=Mat_<uchar>::zeros(mo1.size());
//        faceRegion.setTo(0);
//        fillConvexFromPoints(faceRegion, vTar, 1);
//        mo1.copyTo(mTmp1, faceRegion);
//        mo2.copyTo(mTmp2, faceRegion);
//        mTmp3 = mTmp1 * (1-di) + mTmp2 * di;
//        imshow("mTmp1", mTmp1);
//        imshow("mTmp2", mTmp2);
//        imshow("mTmp3", mTmp3);
//        mTmp3.copyTo(mo1, faceRegion);
//        imshow("mo1_new", mo1);
//        imshow("mo2", mo2);
//        cvWaitKey();
//    }

    ModelImage m, nm;
    FitResult res, nres, rest;

    vector< Point2i > v2, nv2;
    v2.insert(v2.begin(), fittedPointV[0].begin()+7, fittedPointV[0].end());
    nv2.insert(nv2.begin(), fittedPointV[1].begin()+7, fittedPointV[1].end());
    nm.loadTrainImage(img[1]);
    nm.initPointsByVector(v2);

    m.loadTrainImage(img[0]);
    m.initPointsByVector(v2);

    res = samModel.getModelParam(m);
    nres = samModel.getModelParam(nm);

    for (int i=0; i<vDiv; i++){
        di = i/(double)vDiv;
        rest.params = res.params * (1-di) + nres.params * di;

        Mat img;
        img = samModel.restoreImage(rest.params, NULL, NULL, cvSize(300, 300));
        cv::imshow("reGenImg", img);
        cvWaitKey(100);
//        cv::imshow("oriImg", m.getTrainImage());
    }

//    Mat imgNew = warpObj.genNewImg(mCartoon.getTrainImage(), 1);
}
