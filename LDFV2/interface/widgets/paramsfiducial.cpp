#include "paramsfiducial.h"
#include "ui_paramsfiducial.h"

#include "mv/tools/mvfiducial.h"
#include "util/dlgkeyboard.h"

/// ===========================================================================
///
/// ===========================================================================
ParamsFiducial::ParamsFiducial(MvFiducial *p, QWidget *parent) : QWidget(parent),
    ui(new Ui::ParamsFiducial), pTool(p)
{
    ui->setupUi(this);
    ui->le_name->installEventFilter( this );
}

/// ===========================================================================
///
/// ===========================================================================
ParamsFiducial::~ParamsFiducial()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    if( pTool )
    {
        ui->btn_angle->setChecked( pTool->GetLockAngle() );
        ui->btn_visible->setChecked( pTool->isVisible() );
        ui->le_name->setText( pTool->GetToolName() );
    }
}

/// ===========================================================================
///
/// ===========================================================================
bool ParamsFiducial::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->le_name && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
           ui->le_name->setText(dlg.getText());
           if(pTool) pTool->SetToolName( ui->le_name->text() );
           return true;
        }
    }
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::SetRunMode()
{
    ui->btn_angle->setVisible( false );
    //ui->frm_contrast->setVisible( false );
}


/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_visible_toggled(bool checked)
{
    if(pTool) pTool->setVisible( checked );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_angle_toggled(bool checked)
{
    if( pTool )
    {
        if( checked ) pTool->setRotation(0);
        pTool->SetLockAngle( checked );
        pTool->Clear();
        pTool->update();

    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_minus_similarity_clicked()
{
    int v = ui->lbl_similarity->text().left( ui->lbl_similarity->text().size() - 1 ).toInt();
    if( v <= 30 ) return;

    v--;

    ui->lbl_similarity->setText(QString::number(v) + "%");

    if( pTool ) pTool->SetMinimumSimilarity( v );

}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_plus_similarity_clicked()
{
    int v = ui->lbl_similarity->text().left( ui->lbl_similarity->text().size() - 1 ).toInt();
    if( v > 98 ) return;

    v++;

    ui->lbl_similarity->setText(QString::number(v) + "%");

    if( pTool ) pTool->SetMinimumSimilarity( v );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_plus_region_clicked()
{
    int v = ui->lbl_region->text().left( ui->lbl_region->text().size() - 1 ).toInt();
    if( v > 70) return;

    v++;

    ui->lbl_region->setText(QString::number(v) + "%");

    if( pTool ) pTool->SetMinimumRegion(v);
}

/// ===========================================================================
///
/// ===========================================================================/
void ParamsFiducial::on_btn_minus_region_clicked()
{
    int v = ui->lbl_region->text().left( ui->lbl_region->text().size() - 1 ).toInt();
    if( v <= 5) return;

    v--;

    ui->lbl_region->setText(QString::number(v) + "%");

    if( pTool ) pTool->SetMinimumRegion(-v);
}
