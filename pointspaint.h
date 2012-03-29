#ifndef POINTSSHOW_H
#define POINTSSHOW_H

#include <QObject>
#include <QGraphicsItem>
#include <QList>
#include <QPoint>

#include "shapemodel.h"
using StatModel::ShapeInfo;

class PointsPaint : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    PointsPaint();

    QRectF boundingRect() const;
    // QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);


    int pickPointByMouse(const QPointF &mouseP) const;
    int getSize() const{ return ql.size(); }
    void addPoint(const QPointF &mouseP);
    void updateSelectedPoint(const QPointF &mouseP);

    void setShapeInfo(ShapeInfo *s){shapeInfo=s;}

    void highLight(int pId);
    void selectPoint(int pId);
    void setMirrorPoint(int pId);

    void setRatio(double r){ ratio = r; }

    void clear();
    void setColorScheme(int schemeId);

    QList< QPoint > getPointList() const { return ql; }

private:
    int selectedPoint;
    int mirrorPoint;
    int mouseHighlightPoint;

    QRect boundRect;

    void testUpdateRect(const QPoint &qp);

    QList< QPoint > ql;

    // Colors
    QColor colorHighlightPen, colorHighlightBrush,
            colorSelectedPen, colorSelectedBrush,
            colorMirrorPen, colorMirrorBrush,
            colorPen, colorBrush;

    double ratio;

    ShapeInfo *shapeInfo;
};

#endif // POINTSSHOW_H
