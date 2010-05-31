#include "photoview.h"
#include<QMouseEvent>
#include<iostream>
#include<QGraphicsView>
using std::cout;
using std::endl;

PhotoView::PhotoView(QWidget *parent)
    : QGraphicsView(parent){
    this->setMouseTracking(true);
    setScene(&gScene);
    //gScene.addRect(0, 0, 90, 90);
    scaleFactor = 1;
    pixItem = NULL;
    dragging = false;
    //pointPaint.ql = &selectedPoints;


    // this->scale(scaleFactor, scaleFactor);
    this->setRenderHint(QPainter::Antialiasing);
    this->setRenderHint(QPainter::SmoothPixmapTransform);

    gScene.addItem(&pointPaint);
    pointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());

    gScene.addItem(&controlPoints);
    controlPoints.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());

    gScene.addItem(&oriPointPaint);
    oriPointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());

    this->imgVisible = true;
    this->markPointVisible = true;
    controlVisible = true;

    this->curMode = Warping;
    //this->fitInView(QRect(QPoint(0, 0), img->size()));
    //this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

PhotoView::~PhotoView()
{
    gScene.removeItem(&pointPaint);
    gScene.removeItem(&oriPointPaint);
    gScene.removeItem(&controlPoints);
}

void PhotoView::zoomIn(){
    scaleFactor *= 1.2;
    this->scale(1.2, 1.2);
    this->setRenderHint(QPainter::SmoothPixmapTransform);
    pointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());
    oriPointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());
    controlPoints.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());
    this->update();
}

void PhotoView::zoomOut(){
    scaleFactor /= 1.2;
    this->scale(1/1.2, 1/1.2);
    pointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());
    oriPointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());
    controlPoints.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());
    this->update();
}

void PhotoView::mousePressEvent(QMouseEvent *event){
    QPointF qp = this->mapToScene(event->x(),event->y());

    // Interactive image warping
    if (this->curMode == PhotoView::Warping){
        if (event->button() == Qt::RightButton){
            controlPoints.cancelLast();
            emit warpControlUpdated();
        }
        // Case 1: Confirm/Cancel the warp
        if (dragging){
            dragging = false;
        }
        // Case 2: Start warping
        else if (event->button() == Qt::LeftButton){
            WarpControlSelected wcs = this->controlPoints.pickPointByMouse(qp);
            if (wcs.controlId != -1){
                controlPoints.select(wcs);
                dragging = true;
            }
            else {
                qDebug("Adding Point...");
                controlPoints.addPoint(qp);
                dragging = true;
    //            controlPoints.selectPoint(-1);
            }
        }
    }
    else if (this->curMode == this->AdjustingMarkPoints) {
        int tI = oriPointPaint.pickPointByMouse(qp);
        if (tI != -1){
            oriPointPaint.selectPoint(tI);
            emit selectedPointUpdated(tI);
            dragging = true;
        }
//        else {
//            pointPaint.addPoint(qp);
//            pointPaint.selectPoint(-1);
//            emit selectedPointUpdated(pointPaint.getSize());
//        }
    }

    //QGraphicsView::mousePressEvent(event);
}

void PhotoView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF qp = this->mapToScene(event->x(),event->y());
    if (this->curMode == this->Warping){
        if ( dragging ){
            this->controlPoints.updateSelectedPoint(qp);
            emit warpControlUpdated();
            return;
        }

        WarpControlSelected wcs = this->controlPoints.pickPointByMouse(qp);
        controlPoints.highLight(wcs);
    }
    else if (this->curMode == this->AdjustingMarkPoints){
        if ( dragging ){
            oriPointPaint.updateSelectedPoint(this->mapToScene(event->pos()));
            emit pointsUpdated();
            return;
        }

        int tI = oriPointPaint.pickPointByMouse(qp);
        oriPointPaint.highLight(tI);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void PhotoView::setImage(const QString &imgFileName)
{
    setImage(QImage(imgFileName));
}

void PhotoView::setImage(const QImage & img)
{
//    imageFileName = imgFileName;
//    QImage *img = new QImage(imgFileName);
//    this->setBackgroundBrush(*img);

    if (pixItem != NULL){
        gScene.removeItem(pixItem);
        delete pixItem;
    }

//    pixData = QPixmap(imgFileName);//, NULL, Qt::SmoothTransformation);
    pixData = QPixmap::fromImage(img);
    pixItem = gScene.addPixmap(pixData);
    pixItem->setVisible(this->imgVisible);
    pixItem->setTransformationMode(Qt::SmoothTransformation);
//    this->setSceneRect(QRect(QPoint(0, 0), img->size()));
//    gScene.setSceneRect(QRect(QPoint(0, 0), img->size()));
//
    this->setSceneRect(QRect(QPoint(0, 0), pixData.size()));
    gScene.setSceneRect(QRect(QPoint(0, 0), pixData.size()));
    this->update();
}

void PhotoView::mouseReleaseEvent(QMouseEvent *event)
{
    // When interactive warping, mouse can release
    if (curMode != Warping)
        dragging = false;
}

void PhotoView::setMirrorPoint(int pId)
{
    pointPaint.setMirrorPoint(pId);
}

void PhotoView::setPointList(const QList< QPoint > &ql){
    pointPaint.clear();
    for (int i=0;i<ql.size();i++)
        pointPaint.addPoint(QPointF(ql[i]));
    emit pointsUpdated();
}

void PhotoView::setOriPointList(const QList< QPoint > &ql){
    oriPointPaint.clear();
    for (int i=0;i<ql.size();i++)
        oriPointPaint.addPoint(QPointF(ql[i]));
    emit pointsUpdated();
}

void PhotoView::setImageVisibility(bool visible)
{
    if (pixItem != NULL){
        pixItem->setVisible(visible);
        this->imgVisible = visible;
    }
}

void PhotoView::setMarkPointVisibility(bool visible)
{
    pointPaint.setVisible(visible);
}

void PhotoView::setMode(ViewMode mode)
{
    curMode = mode;
    if (curMode == PhotoView::Warping){
        controlPoints.setVisible(true);
    }
    else
        controlPoints.setVisible(false);

    if (curMode == PhotoView::AdjustingMarkPoints)
        this->oriPointPaint.setVisible(true);
}

//! Export the scene to an Image file.
void PhotoView::exportImage(const QString & imgFileName)
{
    QImage img(gScene.sceneRect().size().toSize(),QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    gScene.render(&painter);
    img.save(imgFileName);
}
