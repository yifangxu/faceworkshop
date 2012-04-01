#include "gui_paramdeform.h"
#include "ui_gui_paramdeform.h"

#include "imgwarp_mls.h"
#include "imgwarp_mls_rigid.h"
#include "imgwarp_mls_similarity.h"
#include "imgwarp_piecewiseaffine.h"

#include "imageprocess.h"

#include <QFileDialog>

GUI_ParamDeform::GUI_ParamDeform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI_ParamDeform)
{
    string appPath = QApplication::applicationDirPath().toLocal8Bit().data();
    ui->setupUi(this);
    ui->wWarpParam->controlPaint = &(ui->viewPic->controlPoints);

    bDeformPic = false;
//     asmModel.readTrainData((appRoot+"data/feret_shape/pts.list").c_str());
    string modelPath;
    modelPath = appPath + "/data/grayall_asm.model";
    cout << "Trying to load model file from: " << modelPath << endl;
    asmModel.loadFromFile(modelPath);

    string faceCascadePath= appPath + "/data/haarcascade_frontalface_alt.xml";
    cout << "Trying to load face detector from: " << faceCascadePath << endl;
    if (!faceCascade.load(faceCascadePath)) {
        std::cerr << "NO face detector found!!!" << endl;
    }

    // Removed to remove dependence on SAM.
    //ui->widgetFaceMorph->setASMModel(&asmModel);
    //ui->widgetFaceMorph->setFaceClassifier(&faceCascade);

    ui->viewPic->pointPaint.setShapeInfo(&asmModel.getShapeInfo());

    ui->cmbImgWarpAlg->addItem(tr("Piece-Wise + MLS"));
    ui->cmbImgWarpAlg->addItem(tr("All Piece-Wise"));
    ui->cmbImgWarpAlg->addItem(tr("MLS Rigid"));
    ui->cmbImgWarpAlg->addItem(tr("MLS Rigid+Scale"));
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

    img = cv::imread(fileName.toLocal8Bit().data());
    if (!img.empty()){
        loadImg(img);
        QFileInfo fi(fileName);
        this->setWindowTitle(this->windowTitle()+" "+fi.fileName());
    }
}
void GUI_ParamDeform::loadImg(Mat& img)
{
    oriImg = img;
    QTime timeObj;
    timeObj.start();
    vector< cv::Rect > faces;
    faceCascade.detectMultiScale(
            img, faces,
            1.2, 2, CV_HAAR_SCALE_IMAGE, Size(60, 60) );
    qDebug("Face detection time: %dms", timeObj.elapsed());

    timeObj.restart();
    fitResV = asmModel.fitAll(oriImg, faces, 0);
    qDebug("ASM fitting time: %dms", timeObj.elapsed());

    // If a face is found, initialize related controllers.
    MyImage mImg=MyImage::fromMat(oriImg);
    ui->viewPic->setImage(mImg);
    if (fitResV.size()>0){
        fitResV[0].toPointList(fittedPointV);
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
//    double fatthinL[]={-0.0286099, 0.157105, -0.0663111, 6.08284e-05, 0.142057, -0.124677, -0.00403317, 0.0185718, 0.0047351, -0.0421543, 0.00146511, -0.00729767, 0.0145544, 0.0021012, 0.0281285, -0.00719042, -0.00782145, 0.0142164, -0.0126156, -0.0260578, -0.00970037, 0.029682, 0.00464373, -0.0211351, 0.0234257, -0.00962877, 0.00929279, 0.0099329, -0.00408232, -0.00947347, 0.012463, 0.00237411, -0.00995264, 0.00822943, 0.0021841, 0.00224892, 0.00115306, -0.00146569};
    // NOTE: These results are calculated based on grayall_asm.model, and hardcoded...
    double fatthinL[]={0.0040596, 0.0160517, 0.0539527, 0.000812555, -0.000752845, 0.00450181, -0.0192521, 0.0196133, 0.0101444, 0.00729279, -0.0162042, -0.0071276, -0.00141391, -0.00201894, -0.00144492, -0.00190538, -0.000497974, -0.00159142, -0.00517663, 0.0056537, 0.00156158, 0.00316443, -0.000180146, -0.00132939, -0.0061002, 0.0010544, 0.000380441, -0.000538201, -0.00398194, -0.00222475, -0.000490223, 0.00116845, 0.000457822, 0.00723525, 0.00514677, 0.00059638, -0.000444628, -0.000535828, -0.00525137, -0.000303542, -0.00199471};
    double eyeSizeL[]={0.00676141, -0.0392785, -0.0518178, -0.0704076, -0.0467006, 0.00772958, 0.00773171, 0.0389458, -0.0834136, 0.00701163, 0.0236421, 0.0651696, -0.0363207, 0.03637, 0.0226069, 0.0393349, 0.0105077, 0.0266615, -0.00760716, 0.0440098, -0.0167599, 0.0170333, -0.00425014, 0.0188851, -0.0314073, 0.0202786, 0.0484056, 0.0056476, -0.0256255, -0.0767187, -0.0169115, -0.00414511, -0.0320143, -0.0345155, -0.0620099, 0.0270108, -0.0144122, -0.00967341, 0.00287082, -0.0119444, -0.0461817};
    //double fatthinL[]={0.040596, 0.0160517, 0.0039527, 0.000812555, -0.000752845, 0.00450181, -0.0192521, 0.0196133, 0.0101444, 0.00729279, -0.0162042, -0.0071276, -0.00141391, -0.00201894, -0.00144492, -0.00190538, -0.000497974, -0.00159142, -0.00517663, 0.0056537, 0.00156158, 0.00316443, -0.000180146, -0.00132939, -0.0061002, 0.0010544, 0.000380441, -0.000538201, -0.00398194, -0.00222475, -0.000490223, 0.00116845, 0.000457822, 0.00723525, 0.00514677, 0.00059638, -0.000444628, -0.000535828, -0.00525137, -0.000303542, -0.00199471};

    vector< cv::Point2i > newPV;
    Mat newImg;

    int processId;
    processId = ui->tabProcessMethod->currentIndex();
    if (processId == 0){
        // Free deformation:
        vector< WarpControl > vC = ui->viewPic->getControlPointList().toVector().toStdVector();
        ui->wWarpParam->processImage(asmModel, vC, fittedPointV, newPV);
        ui->viewPic->setControlVisibility(ui->wWarpParam->showControl);
    }
    else if (processId == 1){
        // Parameterized deformation:
        Mat_<double> m2=ui->wParamEdit->getParamV().clone();

		m2 *= 1 + double(ui->sldDeidentify->value()) / ui->sldDeidentify->maximum();
        for (int i=0; i<38; i++){
            m2(i, 0)+=fatthinL[i]*ui->sldFatThin->value()*0.5;
            m2(i, 0)+=eyeSizeL[i]*ui->sldEyeSize->value()*0.15;
        }

        fitResV[0].params = asmModel.reConFromNorm(m2);
        fitResV[0].toPointList(newPV);
    }
    else {
        // Edit parameters manually.
        Mat_<double> m2=ui->wParamEdit->getParamV().clone();

        fitResV[0].params = asmModel.reConFromNorm(m2);
        fitResV[0].toPointList(newPV);
    }

    // New landmark points should be in newPV.
    ui->viewPic->setPointList(getQListQPoint(newPV));

    if (bDeformPic){
        // Actually warp image!
        ImgWarp_MLS *warp;
        int imgWarpAlg = ui->cmbImgWarpAlg->currentIndex();
        if (imgWarpAlg==0){
            warp = new ImgWarp_PieceWiseAffine;
            ((ImgWarp_PieceWiseAffine *) warp)->backGroundFillAlg = ImgWarp_PieceWiseAffine::BGMLS;
        }
        else if (imgWarpAlg==1){
            warp = new ImgWarp_PieceWiseAffine;
            ((ImgWarp_PieceWiseAffine *) warp)->backGroundFillAlg = ImgWarp_PieceWiseAffine::BGPieceWise;
        }
        else if (imgWarpAlg==2)
            warp = new ImgWarp_MLS_Rigid;
        else if (imgWarpAlg == 3){
            warp = new ImgWarp_MLS_Rigid;
            ((ImgWarp_MLS_Rigid *)warp)->preScale = true;
        }
        else
            warp = new ImgWarp_MLS_Similarity;

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
    }
    else{
        newImg = oriImg;
        ui->viewPic->setImage(MyImage::fromMat(newImg));
    }
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

void GUI_ParamDeform::on_actionSaveOriImage_triggered()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
               tr("Save original image..."), "./", tr("Image Files (*.jpg *.png *.ppm);;All Files (*.*)"));


    cv::imwrite(fileName.toLocal8Bit().data(), oriImg);
}

void GUI_ParamDeform::on_actionExport_triggered()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
               tr("Export image..."), "./", tr("Image Files (*.jpg *.png *.ppm);;All Files (*.*)"));

    ui->viewPic->exportImage(fileName);
}
