#ifndef WIDGET_MODELPARAMEDIT_H
#define WIDGET_MODELPARAMEDIT_H

#include <QWidget>
#include "cv.h"
using cv::Mat_;

namespace Ui {
    class Widget_ModelParamEdit;
}

class Widget_ModelParamEdit : public QWidget {
    Q_OBJECT
public:
    Widget_ModelParamEdit(QWidget *parent = 0);
    ~Widget_ModelParamEdit();

    void setParamV(Mat_<double> &V);
    Mat_<double> getParamV(){ return paramV; }

protected:
    void changeEvent(QEvent *e);

    Mat_<double> paramV;
    int curParam;
    int divs;

private:
    Ui::Widget_ModelParamEdit *ui;

signals:
    void paramUpdated();

private slots:
    void on_dialParam_valueChanged(int value);
    void on_sldSelectParam_valueChanged(int value);
};

#endif // WIDGET_MODELPARAMEDIT_H
