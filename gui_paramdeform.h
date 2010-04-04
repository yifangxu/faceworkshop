#ifndef GUI_PARAMDEFORM_H
#define GUI_PARAMDEFORM_H

#include <QMainWindow>
#include <string>
#include <vector>
#include "myimage.h"
using std::string;
using std::vector;
#include "asmmodel.h"

namespace Ui {
    class GUI_ParamDeform;
}

class GUI_ParamDeform : public QMainWindow {
    Q_OBJECT
public:
    GUI_ParamDeform(QWidget *parent = 0);
    ~GUI_ParamDeform();

public slots:
    void updatePic();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GUI_ParamDeform *ui;

    ASMModel asmModel;

    vector< FitResult > fitResV;
    Mat oriImg;

    vector< cv::Point2i > fittedPointV;

    cv::CascadeClassifier faceCascade;

    bool bDeformPic;

    void loadImg(Mat& img);

private slots:
    void on_tabProcessMethod_currentChanged(int index);
    void on_actionZoom_out_triggered();
    void on_actionZoom_In_triggered();
    void on_actCapFromCam_triggered();
    void on_chkDeform_toggled(bool checked);
    void on_wParamEdit_paramUpdated();
    void on_actionLoadImg_triggered();
};

#endif // GUI_PARAMDEFORM_H
