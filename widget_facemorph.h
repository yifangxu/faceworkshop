#ifndef WIDGET_FACEMORPH_H
#define WIDGET_FACEMORPH_H

#include <QWidget>
#include "asmmodel.h"
#include "aammodel.h"

namespace Ui {
    class Widget_FaceMorph;
}

class Widget_FaceMorph : public QWidget {
    Q_OBJECT
public:
    Widget_FaceMorph(QWidget *parent = 0);
    ~Widget_FaceMorph();

    ASMModel asmModel;
    AAMModel samModel;

    vector< FitResult > fitResV[2];
    Mat img[2];

    vector< cv::Point2i > fittedPointV[2];
    cv::CascadeClassifier faceCascade;

    void loadImg(Mat& img, int i);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Widget_FaceMorph *ui;

private slots:
    void on_btnRender_clicked();
    void on_btnLoadImg2_clicked();
    void on_btnCapImg1_clicked();
};

#endif // WIDGET_FACEMORPH_H
