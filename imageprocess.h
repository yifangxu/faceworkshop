#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "cv.h"
using cv::Point2i;
using cv::Vec2i;

#include <vector>
using std::vector;

#include "asmmodel.h"
using StatModel::ASMModel;
using StatModel::ASMFitResult;

//! Free deformation is described by a list WarpControl's.
class WarpControl
{
public:
    //! A point on (original image)
    Point2i oriPoint;

    //! A deformation vector based on that point.
    Vec2i warpVec;

    //! How much does a point affect its neighbour?
    double r;
};

class ImageProcess
{
public:
    ImageProcess();
};

void interactiveImageWarping(
        const vector< WarpControl > &controlList,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints);

void stepByStepImageWarping(
        const vector< WarpControl > &controlList,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints);

void interactiveStableImageWarping(
        const vector< WarpControl > &controlList,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints);

void pointStableImageWarping(
        const vector< WarpControl > &controlList,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints);

//! Project a point list to Shape Model, then find the model approximation
ASMFitResult projectPListToShapeModel(
        ASMModel &model,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints
        );

#endif // IMAGEPROCESS_H
