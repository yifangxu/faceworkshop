#ifndef WIDGET_WARPCONTROLPAINT_H
#define WIDGET_WARPCONTROLPAINT_H

#include <QObject>
#include <QGraphicsItem>

#include "imageprocess.h"


struct WarpControlSelected{
    enum SelectedType{Selected_ORI_Point, Selected_TAR_Point};

    WarpControlSelected():controlId(-1), type(Selected_ORI_Point){}
    int controlId;

    //! type: 0-ori point;  1-tar point
    SelectedType type;

    bool operator !=(const WarpControlSelected &b) const{
        return !(controlId==b.controlId && type==b.type);
    }
};

class Widget_WarpControlPaint :  public QObject, public QGraphicsItem
{
public:
    Widget_WarpControlPaint();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);


    //! Pick a point around the cursor.
    WarpControlSelected pickPointByMouse(const QPointF &mouseP);

    //! Add a new point into the list
    void addPoint(const QPointF &mouseP);

    //! Update the position of the selected point by cursor.
    void updateSelectedPoint(const QPointF &mouseP);

    void highLight(WarpControlSelected &wcs);
    void select(WarpControlSelected &wcs);
//    void setMirrorPoint(int pId);

    void setRatio(double r){ ratio = r; }

    void clear();
    void cancelLast();
    void setColorScheme(int schemeId);

    QList< WarpControl > getControlPointList() const { return warpCList; }

private:
    WarpControlSelected selectedControl;
    WarpControlSelected highlightControl;

    QRect boundRect;

    void testUpdateRect(const QPoint &qp);

    QList< WarpControl > warpCList;

    // Colors
    QColor colorHighlightPen, colorHighlightBrush,
            colorSelectedPen, colorSelectedBrush,
            colorTarPen, colorTarBrush,
            colorSelectedTarPen, colorSelectedTarBrush,
            colorPen, colorBrush;

    double ratio;
};

#endif // WIDGET_WARPCONTROLPAINT_H
