#include "gui_paramdeform.h"
#include "ui_gui_paramdeform.h"

#include "imgtrans_mls_rigid.h"
#include "imgtrans_mls_similarity.h"
#include "imgtranspiecewiseaffine.h"

#include "imageprocess.h"

#include <QFileDialog>

GUI_ParamDeform::GUI_ParamDeform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI_ParamDeform)
{
    string appPath = QApplication::applicationDirPath().toLocal8Bit().data();
    ui->setupUi(this);

    bDeformPic = false;
//     asmModel.readTrainData((appRoot+"data/feret_shape/pts.list").c_str());
    string modelPath;
    modelPath = appPath + "/data/color_asm68_ascii.model";
    asmModel.load(modelPath);

    string faceCascadePath= appPath + "/data/haarcascade_frontalface_alt.xml";
    faceCascade.load(faceCascadePath);

    ui->widgetFaceMorph->setASMModel(&asmModel);
    ui->widgetFaceMorph->setFaceClassifier(&faceCascade);

    ui->viewPic->pointPaint.setShapeInfo(&asmModel.getShapeInfo());

    ui->cmbImgWarpAlg->addItem(tr("Piece-Wise Warping"));
    ui->cmbImgWarpAlg->addItem(tr("MLS Rigid"));
    ui->cmbImgWarpAlg->addItem(tr("MLS Similarity"));

    connect(&threadWarp, SIGNAL(finished()), this, SLOT(showWarpRes()));
}

GUI_ParamDeform::~GUI_ParamDeform()
{
    delete ui;
}

void GUI_ParamDeform::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GUI_ParamDeform::on_actionLoadImg_triggered()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
               tr("Open List file"), "./", tr("Image Files (*.jpg *.png *.ppm);;All Files (*.*)"));

    Mat img;
    
    img = cv::imread(fileName.toStdString());
    if (!img.empty())
        loadImg(img);
}
void GUI_ParamDeform::loadImg(Mat& img)
{
    oriImg = img;
    QTime timeObj;
    timeObj.start();
    asmModel.fit(oriImg, fitResV, faceCascade, true, 0);
    qDebug("ASM Time: %d", timeObj.elapsed());
//    asmModel.showResult(img, fitResult);

    MyImage mImg=MyImage::fromMat(oriImg);
    ui->viewPic->setImage(mImg);
    if (fitResV.size()>0){
        asmModel.resultToPointList(fitResV[0], fittedPointV);
        ui->viewPic->setPointList(getQListQPoint(fittedPointV));
        Mat_<double> m2=asmModel.normalizeParam(fitResV[0].params);
        ui->wParamEdit->setParamV(m2);
    }
}

void GUI_ParamDeform::on_wParamEdit_paramUpdated()
{
    updatePic();
}

void GUI_ParamDeform::updatePic(){
    if (fitResV.size()==0)
        return;
//    double fatthinL[]={-0.00358229, 0.0303741, -0.00398948, 0.0422324, 0.0177682, 0.0240675, 0.000506884, 0.00171348, 0.00286941, -0.00348525, -0.00198361, 0.00348873, 0.0010854, 0.00370192, -0.00327463, -0.00684588, 0.0043453, 0.00280502, 0.00170437, 0.000480568, -0.00267232, -0.00214505, 0.002633, 0.000794968, 0.000604818, -0.0031604, -0.000393982, -0.00128753, -0.000211793, 0.00164599, 0.00196191, -0.000473628, 0.00183526, 0.0011174};
    double fatthinL[]={-0.0286099, 0.157105, -0.0663111, 6.08284e-05, 0.142057, -0.124677, -0.00403317, 0.0185718, 0.0047351, -0.0421543, 0.00146511, -0.00729767, 0.0145544, 0.0021012, 0.0281285, -0.00719042, -0.00782145, 0.0142164, -0.0126156, -0.0260578, -0.00970037, 0.029682, 0.00464373, -0.0211351, 0.0234257, -0.00962877, 0.00929279, 0.0099329, -0.00408232, -0.00947347, 0.012463, 0.00237411, -0.00995264, 0.00822943, 0.0021841, 0.00224892, 0.00115306, -0.00146569};

    vector< cv::Point2i > newPV;
    Mat newImg;

    int processId;
    processId = ui->tabProcessMethod->currentIndex();
    if (processId == 0){
        vector< WarpControl > vC = ui->viewPic->getControlPointList().toVector().toStdVector();
        ui->wWarpParam->processImage(asmModel, vC, fittedPointV, newPV);
    }
    else if (processId == 1){
        Mat_<double> m2=ui->wParamEdit->getParamV().clone();

        for (int i=0; i<38; i++)
            m2(i, 0)+=fatthinL[i]*ui->sldFatThin->value()*0.1;

        fitResV[0].params = asmModel.reConFromNorm(m2);
        asmModel.resultToPointList(fitResV[0], newPV);
    }
    else {
        Mat_<double> m2=ui->wParamEdit->getParamV().clone();

        fitResV[0].params = asmModel.reConFromNorm(m2);
        asmModel.resultToPointList(fitResV[0], newPV);
    }
//    interactiveStableImageWarping( vC,
//                             fittedPointV, newPV );


    ui->viewPic->setPointList(getQListQPoint(newPV));

    if (bDeformPic){
        ImgTrans_MLS *warp;
        int imgWarpAlg = ui->cmbImgWarpAlg->currentIndex();
        if (imgWarpAlg==0)
            warp = new ImgTransPieceWiseAffine;
        else if (imgWarpAlg==1)
            warp = new ImgTrans_MLS_Rigid;
        else
            warp = new ImgTrans_MLS_Similarity;

//        ImgTransPieceWiseAffine warpObj;
        warp->setTargetSize(oriImg.cols, oriImg.rows);
        warp->setSize(oriImg.cols, oriImg.rows);
        warp->setSrcPoints(fittedPointV);
        warp->setDstPoints(newPV);
        warp->alpha = 1;
        warp->gridSize = 10;

        if (this->threadWarp.updateWarpPtr(warp)){
            // Will be deleted by the thread
            threadWarp.setImage(oriImg);
            threadWarp.start();
        }
        else
            delete warp;
//        warp->calcDelta();
//        newImg = warp->genNewImg(oriImg, 1);

//        delete warp;
    }
    else{
        newImg = oriImg;
        ui->viewPic->setImage(MyImage::fromMat(newImg));
    }

//    projectPListToShapeModel(asmModel, newPV, uPV);
//    newPV = uPV;
//
////    asmModel.resultToPointList(fitResV[0], newPV);
//    ui->viewPic->setPointList(getQListQPoint(newPV));
//
////    ImgTrans_MLS_Rigid warpObj;
//    if (bDeformPic){
//        ImgTransPieceWiseAffine warpObj;
//        warpObj.setTargetSize(oriImg.cols, oriImg.rows);
//        warpObj.setSize(oriImg.cols, oriImg.rows);
//        warpObj.setSrcPoints(fittedPointV);
//        warpObj.setDstPoints(newPV);
//        warpObj.alpha = 1;
//        warpObj.gridSize = 10;
//        warpObj.calcDelta();
//        Mat imgNew = warpObj.genNewImg(oriImg, 1);
//        ui->viewPic->setImage(MyImage::fromMat(imgNew));
//    }
//    else
//        ui->viewPic->setImage(MyImage::fromMat(oriImg));
}

void GUI_ParamDeform::showWarpRes()
{
    ui->viewPic->setImage(MyImage::fromMat(this->threadWarp.getImage()));
}

void GUI_ParamDeform::on_chkDeform_toggled(bool checked)
{
    this->bDeformPic = checked;
    updatePic();
}

#include "gui_videocapture.h"
void GUI_ParamDeform::on_actCapFromCam_triggered()
{
    GUI_VideoCapture gvc(this);
    gvc.exec();
    loadImg(gvc.img);
}

void GUI_ParamDeform::on_actionZoom_In_triggered()
{
    ui->viewPic->zoomIn();
}

void GUI_ParamDeform::on_actionZoom_out_triggered()
{
    ui->viewPic->zoomOut();
}

void GUI_ParamDeform::on_tabProcessMethod_currentChanged(int index)
{
    if (index == 0)
        ui->viewPic->setMode(PhotoView::Warping);
    else
        ui->viewPic->setMode(PhotoView::ViewOnly);
}
