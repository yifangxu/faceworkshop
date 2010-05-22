#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "cv.h"
using cv::Point2i;
using cv::Vec2i;

#include <vector>
using std::vector;

#include "asmmodel.h"

class WarpControl
{
public:
    Point2i oriPoint;
    Vec2i warpVec;
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
FitResult projectPListToShapeModel(
        ASMModel &model,
        const vector< Point2i > &keyPoints,
        vector< Point2i > & newPoints
        );

#endif // IMAGEPROCESS_H
