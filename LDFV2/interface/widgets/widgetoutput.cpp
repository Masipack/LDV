#include "widgetoutput.h"
#include "ui_widgetoutput.h"
#include "util/serialcontrol.h"


/// ===========================================================================
///
/// ===========================================================================
widgetOutPut::widgetOutPut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widgetOutPut)
{
    ui->setupUi(this);
}

/// ===========================================================================
///
/// ===========================================================================
widgetOutPut::~widgetOutPut()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void widgetOutPut::SetNumberOut(const int n)
{
     ui->lbl_out_num->setText(QString::number(n));
     ui->lbl_Status->setText(tr("Saída %1 OFF").arg(QString::number(ui->lbl_out_num->text().toInt() - 1)));
}

/// ===========================================================================
///
/// ===========================================================================
void widgetOutPut::on_btn_exec_toggled(bool checked)
{
    if(checked)
    {
        ui->lbl_Status->setText(tr("Saída %1 ON").arg(QString::number(ui->lbl_out_num->text().toInt() - 1)));
        ui->btn_exec->setIcon(QIcon(":/icons/idea.png"));

        SerialControl::instance()->ToggleBitON(ui->lbl_out_num->text().toInt() - 1);
        return;
    }

    ui->lbl_Status->setText(tr("Saída %1 OFF").arg(QString::number(ui->lbl_out_num->text().toInt() - 1)));
    ui->btn_exec->setIcon(QIcon(":/icons/idea-alt.png"));
    SerialControl::instance()->ToggleBitOFF(ui->lbl_out_num->text().toInt() - 1);
}
