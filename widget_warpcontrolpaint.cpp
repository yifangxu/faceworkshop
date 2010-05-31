#include "widget_warpcontrolpaint.h"

#include <QPen>
#include <QBrush>
#include <QPainter>

Widget_WarpControlPaint::Widget_WarpControlPaint()
{
    boundRect.setCoords(0, 0, 0, 0);
    setZValue(1);
    setColorScheme(1);
}

void Widget_WarpControlPaint::setColorScheme(int schemeId)
{
    if (schemeId == 0){
        colorBrush = Qt::magenta;
        colorSelectedPen = Qt::yellow;
        colorSelectedBrush = Qt::darkYellow;
        colorHighlightPen = Qt::blue;
        colorTarPen = Qt::darkGreen;
        colorTarBrush = Qt::green;
        colorPen = Qt::black;
    }
    else {
        colorBrush = Qt::gray;
        colorSelectedPen = Qt::green;
        colorSelectedBrush = Qt::darkGreen;
        colorHighlightPen = Qt::red;
        colorTarPen = Qt::yellow;
        colorTarBrush = Qt::darkYellow;
        colorSelectedTarPen = Qt::lightGray;
        colorSelectedTarBrush = Qt::gray;

        colorPen = Qt::darkBlue;
    }
}

QRectF Widget_WarpControlPaint::boundingRect() const{
    return (QRectF)boundRect;
}

//void drawArrow( const QPointF& pt, const QPointF& ppt, double sze, const QPen& pen )
//{
//    QPointF pd, pa, pb;
//    double tangent;
//    /* in order to get the arrowhead the correct way round,
//     * we reverse the gradient.
//     * This is equivalent to reversing the geometry below...
//     */
//    pd.x = ppt.x - pt.x, pd.y = ppt.y - pt.y;
//    if (pd.x == 0 && pd.y == 0)
//        return;
//    tangent = atan2 ((double) pd.y, (double) pd.x);
//    pa.x = sze * cos (tangent + M_PI / 7) + pt.x;
//    pa.y = sze * sin (tangent + M_PI / 7) + pt.y;
//    pb.x = sze * cos (tangent - M_PI / 7) + pt.x;
//    pb.y = sze * sin (tangent - M_PI / 7) + pt.y;
//    //-- connect the dots...
//    CanvasLine* lhs = new CanvasLine( canvas );
//    lhs->setPoints( pa.x, pa.y, pt.x, pt.y );
//    lhs->setPen( pen );
//    lhs->setZ( 1 );
//    lhs->show();
//    SimpleCanvasLine* rhs = new SimpleCanvasLine( canvas, this );
//    rhs->setPoints( pb.x, pb.y, pt.x, pt.y );
//    rhs->setPen( pen );
//    lhs->setZ( 1 );
//    rhs->show();
//}

void Widget_WarpControlPaint::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
    QWidget *){
    int i;
    QPen dPen;
    QBrush dBrush;
    dBrush.setStyle(Qt::SolidPattern);
    dBrush.setColor(colorBrush);
    painter->setBrush(dBrush);

    //double ratio = this->(1,0).x()-this->mapFromScene(0, 0).x();
    //qDebug("%lf\n",ratio);

    for (i = 0; i < warpCList.size(); i++){
        QPointF oriPoint = QPointF(warpCList[i].oriPoint.x, warpCList[i].oriPoint.y);
        QPointF newPoint = QPointF(warpCList[i].oriPoint.x+warpCList[i].warpVec[0],
                                   warpCList[i].oriPoint.y+warpCList[i].warpVec[1]);
        if (selectedControl.controlId == i){
            dPen.setColor(colorSelectedPen);
            painter->setPen(dPen);
            dBrush.setColor(colorSelectedBrush);
            painter->setBrush(dBrush);
            painter->drawEllipse(oriPoint,
                                 4.*ratio, 4.*ratio);
            dBrush.setColor(colorBrush);

            dPen.setColor(colorSelectedTarPen);
            painter->setPen(dPen);
            dBrush.setColor(colorSelectedTarBrush);
            painter->setBrush(dBrush);
            painter->drawEllipse(newPoint, 3.*ratio, 3.*ratio);
            dBrush.setColor(colorSelectedBrush);
            painter->setBrush(dBrush);
        }
        else if (highlightControl.controlId == i){
            dPen.setColor(colorHighlightPen);
            painter->setPen(dPen);
            painter->drawEllipse(oriPoint,
                                 4.*ratio, 4.*ratio);
        }
//        else if (mirrorPoint == i){
//            dPen.setColor(colorMirrorPen);
//            painter->setPen(dPen);
//            dBrush.setColor(colorMirrorBrush);
//            painter->setBrush(dBrush);
//            painter->drawEllipse(QPointF(warpCList[i].oriPoint.x, warpCList[i].oriPoint.y),
//                                 4.*ratio, 4.*ratio);
//            dBrush.setColor(colorBrush);
//            painter->setBrush(dBrush);
//        }
        else {
            dPen.setColor(colorPen);
            painter->setPen(dPen);
            painter->drawEllipse(oriPoint,
                                 3.*ratio, 3.*ratio);


            dPen.setColor(colorTarPen);
            painter->setPen(dPen);
            dBrush.setColor(colorTarBrush);
            painter->setBrush(dBrush);
            painter->drawEllipse(newPoint, 3.*ratio, 3.*ratio);
            dBrush.setColor(colorBrush);
            painter->setBrush(dBrush);
        }

        dBrush.setColor(colorBrush);
        painter->setBrush(dBrush);
        dPen.setColor(colorTarPen);
        painter->setPen(dPen);
        painter->drawLine(oriPoint, newPoint);

    }
}

// Pick a point by mouse. mouseP is in item coordination system.
WarpControlSelected Widget_WarpControlPaint::pickPointByMouse(const QPointF &mouseP){
    double minD=1e10, dist;
    int i, minI;

    WarpControlSelected wcs;
    for (i = 0; i < warpCList.size(); i++){
        double tx, ty;
        tx = warpCList[i].oriPoint.x;
        ty = warpCList[i].oriPoint.y;

        dist = (mouseP.x() - tx)*(mouseP.x() - tx) +
               (mouseP.y() - ty)*(mouseP.y() - ty);
        if ( dist<minD ) {
            minD = dist;
            minI = i;
            wcs.type = WarpControlSelected::Selected_ORI_Point;
        }

        tx = warpCList[i].oriPoint.x + warpCList[i].warpVec[0];
        ty = warpCList[i].oriPoint.y + warpCList[i].warpVec[1];
        dist = (mouseP.x() - tx)*(mouseP.x() - tx) +
               (mouseP.y() - ty)*(mouseP.y() - ty);
        if ( dist<minD ) {
            minD = dist;
            minI = i;
            wcs.type = WarpControlSelected::Selected_TAR_Point;
        }
    }

    // A point is found
    if (minD < 16*ratio*ratio){
        wcs.controlId = minI;
//        wcs.type = WarpControlSelected::Selected_ORI_Point;
    }
    return wcs;
}

void Widget_WarpControlPaint::highLight(WarpControlSelected &wcs){
    if (highlightControl != wcs){
        highlightControl = wcs;
        this->update();
    }
}

void Widget_WarpControlPaint::select(WarpControlSelected &wcs){
    if (selectedControl!=wcs){
        selectedControl = wcs;
        this->update();
    }
}

void Widget_WarpControlPaint::updateSelectedPoint(const QPointF &mouseP){
    QPoint qp = mouseP.toPoint();

    if (selectedControl.type==WarpControlSelected::Selected_ORI_Point){
        if (qp.x() != warpCList[selectedControl.controlId].oriPoint.x ||
                qp.y() != warpCList[selectedControl.controlId].oriPoint.y){
            warpCList[selectedControl.controlId].oriPoint.x = qp.x();
            warpCList[selectedControl.controlId].oriPoint.y = qp.y();
            testUpdateRect(qp);
        }
    }
    else if (selectedControl.type==WarpControlSelected::Selected_TAR_Point){
        double nx, ny;
        nx = qp.x() - warpCList[selectedControl.controlId].oriPoint.x;
        ny = qp.y() - warpCList[selectedControl.controlId].oriPoint.y;

        if (nx != warpCList[selectedControl.controlId].warpVec[0] ||
                ny != warpCList[selectedControl.controlId].warpVec[1]){
            warpCList[selectedControl.controlId].warpVec[0] = nx;
            warpCList[selectedControl.controlId].warpVec[1] = ny;
            testUpdateRect(qp);
        }
    }
    this->update();
}

// Pick a point by mouse. mouseP is in item coordination system.
void Widget_WarpControlPaint::addPoint(const QPointF &mouseP) {
    WarpControl wc;
    wc.oriPoint.x = mouseP.x();
    wc.oriPoint.y = mouseP.y();
    wc.warpVec[0] = wc.warpVec[1] = 0;
    this->warpCList.append(wc);
    this->selectedControl.controlId = warpCList.size()-1;
    this->selectedControl.type = WarpControlSelected::Selected_TAR_Point;
    testUpdateRect(mouseP.toPoint());
    this->update();
}

void Widget_WarpControlPaint::testUpdateRect(const QPoint &qp){
    bool needUpdate = false;
    if (qp.y()+3>boundRect.bottom())
        boundRect.setBottom(qp.y()+3), needUpdate=true;
    if (qp.x()+3>boundRect.right())
        boundRect.setRight(qp.x()+3), needUpdate=true;
    if (needUpdate)
        this->prepareGeometryChange();
}

void Widget_WarpControlPaint::clear()
{
    warpCList.clear();
    highlightControl.controlId = -1;
    selectedControl.controlId = -1;
    boundRect.setCoords(0, 0, 0, 0);
    setZValue(1);
}

void Widget_WarpControlPaint::cancelLast()
{
    if (warpCList.size()>0)
        warpCList.removeLast();
    this->update();
}
