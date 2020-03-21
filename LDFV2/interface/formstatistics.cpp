#include "formstatistics.h"
#include "ui_formstatistics.h"
#include "QDebug"

FormStatistics::FormStatistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormStatistics)
{
    ui->setupUi(this);
}

FormStatistics::~FormStatistics()
{
    delete ui;
}

void FormStatistics::NewInsp(bool v)
{
    int n = ui->lbl_insp->text().toInt();
    n++;
    ui->lbl_insp->setText(QString::number(n));

    if( v )
    {
        int n = ui->lbl_ok->text().toInt();
        n++;
        ui->lbl_ok->setText(QString::number(n));
    }
    else
    {
        int n = ui->lbl_nok->text().toInt();
        n++;
        ui->lbl_nok->setText(QString::number(n));
    }

}

void FormStatistics::on_btn_reset_clicked()
{
    ui->lbl_insp->setText("0");
    ui->lbl_nok->setText("0");
    ui->lbl_ok->setText("0");
}
