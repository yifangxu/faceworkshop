#include "imageprocess.h"

#include <cstdio>
using cv::norm;
using cv::Mat_;

ImageProcess::ImageProcess()
{
}

// This file is UGLY... Functions are actually quite similar.

void interactiveImageWarping(
        const vector< WarpControl > &controlList,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints)
{
    WarpControl w;
    newPoints = keyPoints;
    for (uint iw=0; iw<controlList.size(); iw++){
        w = controlList[iw];
        for (uint i=0; i<keyPoints.size(); i++){
            Point2i curP = keyPoints[i];
            Vec2i pV;
            pV[0] = w.oriPoint.x - curP.x;
            pV[1] = w.oriPoint.y - curP.y;

            double cosT, weight;
            if ((w.warpVec[0]==0 && w.warpVec[1]==0)||
                (pV[0]==0 && pV[1]==0))
                weight = 0;
            else {
                cosT = pV.dot(w.warpVec)/(norm(pV)*norm(w.warpVec));
                weight = exp(-pV.dot(pV)/(pow(2,1-cosT)*8000));
            }

            newPoints[i].x += weight * w.warpVec[0];
            newPoints[i].y += weight * w.warpVec[1];
        }
    }
}

void stepByStepImageWarping(
        const vector< WarpControl > &controlList,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints)
{
    WarpControl w;
    newPoints = keyPoints;
    for (uint iw=0; iw<controlList.size(); iw++){
        w = controlList[iw];
        for (uint i=0; i<keyPoints.size(); i++){
            Point2i curP = newPoints[i];
            Vec2i pV;
            pV[0] = w.oriPoint.x - curP.x;
            pV[1] = w.oriPoint.y - curP.y;

            double cosT, weight;
            if ((w.warpVec[0]==0 && w.warpVec[1]==0)||
                (pV[0]==0 && pV[1]==0))
                weight = 0;
            else {
                cosT = pV.dot(w.warpVec)/(norm(pV)*norm(w.warpVec));
                weight = exp(-pV.dot(pV)/(pow(2,1-cosT)*8000));
            }

            newPoints[i].x += weight * w.warpVec[0];
            newPoints[i].y += weight * w.warpVec[1];
        }
    }
}

void interactiveStableImageWarping(
        const vector< WarpControl > &controlList,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints)
{
    WarpControl w;
    newPoints = keyPoints;
    vector< double > vWeaken(controlList.size());
    for (uint i=0; i<keyPoints.size(); i++){
        double allS = 1;
        // Calculate ON CONTROL POINT weight decline
        for (uint iw=0; iw<controlList.size(); iw++){
            w = controlList[iw];
            Point2i curP = keyPoints[i];
            Vec2i pV;
            pV[0] = w.oriPoint.x - curP.x;
            pV[1] = w.oriPoint.y - curP.y;

            double cosT, weight;
            if ((w.warpVec[0]==0 && w.warpVec[1]==0)||
                (pV[0]==0 && pV[1]==0))
                weight = 1;
            else {
                cosT = pV.dot(w.warpVec)/(norm(pV)*norm(w.warpVec));
                weight = exp(-pV.dot(pV)/(pow(1.1,1-cosT)*80));
            }
            if (fabs(1-weight)>1e-3)
                allS *= 1-weight;
            vWeaken[iw] = 1-weight;
        }

        for (uint iw=0; iw<controlList.size(); iw++){
            w = controlList[iw];
            Point2i curP = keyPoints[i];
            Vec2i pV;
            pV[0] = w.oriPoint.x - curP.x;
            pV[1] = w.oriPoint.y - curP.y;

            double cosT, weight;
            if ((w.warpVec[0]==0 && w.warpVec[1]==0)||
                (pV[0]==0 && pV[1]==0))
                weight = 1;
            else {
                cosT = pV.dot(w.warpVec)/(norm(pV)*norm(w.warpVec));
                weight = exp(-pV.dot(pV)/(pow(1.1,1-cosT)*8000));
            }

            if (fabs(vWeaken[iw])>1e-3)
                weight *= allS/vWeaken[iw];

            newPoints[i].x += weight * w.warpVec[0];
            newPoints[i].y += weight * w.warpVec[1];
        }
    }
}

void pointStableImageWarping(
        const vector< WarpControl > &controlList,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints)
{
    Mat_<double> matLx, matRx, matLy, matRy;

    WarpControl w;
    newPoints = keyPoints;

    // If no control object presents, simply return.
    if (controlList.size() == 0) {
        return;
    }

    matLx.create(controlList.size(), controlList.size());
    matLy.create(controlList.size(), controlList.size());
    matRy.create(controlList.size(), 1);
    matRx.create(controlList.size(), 1);

    for (uint iw=0; iw<controlList.size(); iw++){
        w = controlList[iw];
        for (uint i=0; i<controlList.size(); i++){
            Point2i curP = controlList[i].oriPoint;
            Vec2i pV;
            pV[0] = w.oriPoint.x - curP.x;
            pV[1] = w.oriPoint.y - curP.y;

            double cosT, weight;
            if ((w.warpVec[0]==0 && w.warpVec[1]==0)||
                (pV[0]==0 && pV[1]==0))
                weight = 1;
            else {
                cosT = pV.dot(w.warpVec)/(norm(pV)*norm(w.warpVec));
                weight = exp(-pV.dot(pV)/(pow(1.1,1-cosT)*8000));
            }
//            printf("%lf\n", weight);

            matLx(i, iw) = weight * w.warpVec[0];
            matLy(i, iw) = weight * w.warpVec[1];

//            newPoints[i].x += weight * w.warpVec[0];
//            newPoints[i].y += weight * w.warpVec[1];
        }
        matRx(iw, 0) = w.warpVec[0];
        matRy(iw, 0) = w.warpVec[1];
    }

    Mat_<double> wAnsX, wAnsY;
    cv::solve(matLx, matRx, wAnsX);
    cv::solve(matLy, matRy, wAnsY);

    newPoints = keyPoints;
    for (uint iw=0; iw<controlList.size(); iw++){
        w = controlList[iw];
        for (uint i=0; i<keyPoints.size(); i++){
            Point2i curP = keyPoints[i];
            Vec2i pV;
            pV[0] = w.oriPoint.x - curP.x;
            pV[1] = w.oriPoint.y - curP.y;

            double cosT, weight;
            if ((w.warpVec[0]==0 && w.warpVec[1]==0)||
                (pV[0]==0 && pV[1]==0))
                weight = 1;
            else {
                cosT = pV.dot(w.warpVec)/(norm(pV)*norm(w.warpVec));
                weight = exp(-pV.dot(pV)/(pow(1.1,1-cosT)*8000));
            }
//            printf("%lf\n", weight);

            newPoints[i].x += weight * wAnsX(iw, 0) * w.warpVec[0];
            newPoints[i].y += weight * wAnsY(iw, 0) * w.warpVec[1];
        }
    }
    //w.oriPoint
}

ASMFitResult projectPListToShapeModel(
        ASMModel &model,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints
        )
{
    ASMFitResult fr(&model);
    StatModel::ShapeVec sv;
    sv.fromPointList(keyPoints);
    model.findParamForShape(sv, fr);
    fr.toPointList(newPoints);
    return fr;
}
