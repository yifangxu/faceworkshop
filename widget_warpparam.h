#ifndef WIDGET_WARPPARAM_H
#define WIDGET_WARPPARAM_H

#include <QWidget>
#include "asmmodel.h"
#include "imageprocess.h"

namespace Ui {
    class Widget_WarpParam;
}

class Widget_WarpParam : public QWidget {
    Q_OBJECT
public:
    Widget_WarpParam(QWidget *parent = 0);
    ~Widget_WarpParam();

    void processImage(ASMModel &model,
                    const vector< WarpControl > &warpControls,
                    const vector< Point2i > &oldPoints,
                    vector< Point2i > &newPoints);

    bool showControl;
signals:
    void updated();
    void resetControl();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Widget_WarpParam *ui;

private slots:
    void on_btnReset_clicked();
    void on_chkHumanLike_toggled(bool checked);
    void slotUpdated();
};

#endif // WIDGET_WARPPARAM_H
