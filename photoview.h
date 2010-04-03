#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include <QGraphicsView>
#include "pointspaint.h"
#include "widget_warpcontrolpaint.h"
#include <QPoint>
#include <QObject>
#include <QList>

class PhotoView : public QGraphicsView
{
    Q_OBJECT

public:
    enum ViewMode {Warping, AdjustingMarkPoints, ViewOnly};

    PhotoView(QWidget *parent = 0);
    ~PhotoView();

    void zoomIn();
    void zoomOut();

    void setImage(const QString & imgFileName);
    void setImage(const QImage & img);

    QList< QPoint > getPointList(){ return pointPaint.getPointList(); }
    QList< WarpControl > getControlPointList(){
        return controlPoints.getControlPointList();
    }

    void setMode(ViewMode mode);

    void setPointList(const QList< QPoint > &ql);

    PointsPaint pointPaint;
public slots:
    void setMirrorPoint(int pId);
    void setImageVisibility(bool visible);
    void setMarkPointVisibility(bool visible);

signals:
    void selectedPointUpdated(int pId);
    void pointsUpdated();
    void warpControlUpdated();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QGraphicsScene gScene;

    //! View & Data of warp controling points
    Widget_WarpControlPaint controlPoints;

    // QList< QPoint > selectedPoints;

    double scaleFactor;

private:
    bool dragging;

    ViewMode curMode;
//    QString imageFileName;

    QGraphicsPixmapItem *pixItem;
    QPixmap pixData;

    bool imgVisible, markPointVisible;
};

#endif // PHOTOVIEW_H
