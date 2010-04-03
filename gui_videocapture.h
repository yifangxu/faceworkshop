#ifndef GUI_VIDEOCAPTURE_H
#define GUI_VIDEOCAPTURE_H

#include <QDialog>
#include <QGraphicsScene>

#include "highgui.h"
using cv::VideoCapture;

namespace Ui {
    class GUI_VideoCapture;
}

class GUI_VideoCapture : public QDialog {
    Q_OBJECT
public:
    GUI_VideoCapture(QWidget *parent = 0);
    ~GUI_VideoCapture();

    void stopCapture();
    void startCapture();

    QGraphicsScene gScene;

    cv::Mat img;

protected:
    virtual void changeEvent( QEvent *e );
    virtual void timerEvent( QTimerEvent *event);
    virtual void hideEvent( QHideEvent *e );
private:
    VideoCapture capture;

    int timerId;

private:
    Ui::GUI_VideoCapture *ui;

private slots:
    void on_cmbCamSelect_currentIndexChanged(int index);
};

#endif // GUI_VIDEOCAPTURE_H
