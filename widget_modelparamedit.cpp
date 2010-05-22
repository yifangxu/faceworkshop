#include "widget_modelparamedit.h"
#include "ui_widget_modelparamedit.h"

Widget_ModelParamEdit::Widget_ModelParamEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget_ModelParamEdit)
{
    divs = 30;
    ui->setupUi(this);
    ui->dialParam->setMaximum(divs);
    ui->dialParam->setMinimum(-divs);
    this->setEnabled(false);
}

Widget_ModelParamEdit::~Widget_ModelParamEdit()
{
    delete ui;
}

void Widget_ModelParamEdit::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget_ModelParamEdit::setParamV(Mat_<double> &V)
{
    this->paramV = V.clone();
    ui->sldSelectParam->setMaximum(V.rows);
    ui->sldSelectParam->setValue(0);
    this->setEnabled(true);
    this->curParam = 0;
    ui->dialParam->setValue(cvRound(paramV(0, 0)*divs));
}

void Widget_ModelParamEdit::on_sldSelectParam_valueChanged(int value)
{
    this->curParam = value;
    ui->dialParam->setValue(cvRound(paramV(value, 0)*divs));
}

void Widget_ModelParamEdit::on_dialParam_valueChanged(int value)
{
    this->paramV(curParam, 0) = (double)value/divs;
    emit this->paramUpdated();
}
