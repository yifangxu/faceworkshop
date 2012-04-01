#ifndef FACEPHOTOVIEW_H
#define FACEPHOTOVIEW_H

#include "photoview.h"
#include "asmmodel.h"
#include "myimage.h"

class Widget_FacePhotoView : public PhotoView
{
Q_OBJECT
public:
    explicit Widget_FacePhotoView(QWidget *parent = 0);

    void modelFit();

    vector< ASMFitResult > fitResV;
    Mat oriImg;

    vector< cv::Point2i > getCurMarkPoints(){return getVcvPoint(this->getOriPointList()); }
    //ASMFitResult *updatedRes;

    ASMModel *asmModel;
    cv::CascadeClassifier *faceCascade;

//    void setFaceCascade(cv::CascadeClassifier *fc){
//    };

    void setShapeInfo(ShapeInfo *si){
        this->pointPaint.setShapeInfo(si);
        this->oriPointPaint.setShapeInfo(si);
    }

signals:

public slots:
    void loadImgFromFile();
    void loadImgFromCam();
    void loadAllFromPTS();

private:
};

#endif // FACEPHOTOVIEW_H
