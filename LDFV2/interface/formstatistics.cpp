#include "formstatistics.h"
#include "ui_formstatistics.h"
#include "QDebug"
#include "util/sys_log.h"
#include "interface/windowmanager.h"

/// ===========================================================================
///
/// ===========================================================================
FormStatistics::FormStatistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormStatistics)
{
    ui->setupUi(this);
}

/// ===========================================================================
///
/// ===========================================================================
FormStatistics::~FormStatistics()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
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

/// ===========================================================================
///
/// ===========================================================================
void FormStatistics::on_btn_reset_clicked()
{
    if( P11(tr("Apagando estatistica"), true ) == false ) return;

    ui->lbl_insp->setText("0");
    ui->lbl_nok->setText("0");
    ui->lbl_ok->setText("0");


}

/// ===========================================================================
///
/// ===========================================================================
void FormStatistics::showEvent(QShowEvent *event)
{
    ui->btn_reset->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);

    ui->label_2->setVisible(qApp->property("USE_STATISTIC").toBool());
    ui->label_3->setVisible(qApp->property("USE_STATISTIC").toBool());
    ui->lbl_nok->setVisible(qApp->property("USE_STATISTIC").toBool());
    ui->lbl_ok->setVisible(qApp->property("USE_STATISTIC").toBool());
}
