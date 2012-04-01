#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include <QGraphicsView>
#include "pointspaint.h"
#include "widget_warpcontrolpaint.h"
#include <QPoint>
#include <QObject>
#include <QList>

//! Show photos with landmark points.
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

    //! Export the scene to an Image file.
    void exportImage(const QString & imgFileName);

    QList< WarpControl > getControlPointList(){
        return controlPoints.getControlPointList();
    }

    void setMode(ViewMode mode);

    void setPointList(const QList< QPoint > &ql);

    void setOriPointList(const QList< QPoint > &ql);
    QList< QPoint > getOriPointList(){ return oriPointPaint.getPointList(); }

    PointsPaint pointPaint, oriPointPaint;

    //! View & Data of warp controling points
    Widget_WarpControlPaint controlPoints;
public slots:
    void setMirrorPoint(int pId);
    void setImageVisibility(bool visible);
    void setMarkPointVisibility(bool visible);
    void setControlVisibility(bool visible){ this->controlVisible = visible;
        controlPoints.setVisible(visible);}

signals:
    void selectedPointUpdated(int pId);
    void pointsUpdated();
    void warpControlUpdated();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QGraphicsScene gScene;

    double scaleFactor;

private:
    bool dragging;

    ViewMode curMode;

    QGraphicsPixmapItem *pixItem;
    QPixmap pixData;

    bool imgVisible, markPointVisible;
    bool controlVisible;
};

#endif // PHOTOVIEW_H
