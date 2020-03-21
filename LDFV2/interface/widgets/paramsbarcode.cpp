#include "paramsbarcode.h"
#include "ui_paramsbarcode.h"

#include "mv/tools/mvbarcode.h"
#include "util/dlgkeyboard.h"

/// ===========================================================================
///
/// ===========================================================================
ParamsBarcode::ParamsBarcode(MvBarcode *p, QWidget *parent) :
    QWidget(parent),  ui(new Ui::ParamsBarcode), pTool(p)
{
    ui->setupUi(this);
    ui->le_name->installEventFilter( this );
    ui->le_expected->installEventFilter( this );
}

/// ===========================================================================
///
/// ===========================================================================
ParamsBarcode::~ParamsBarcode()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsBarcode::SetRunMode()
{
    ui->btn_angle->setVisible( false );
    ui->btn_pharmacode->setVisible( false );
    ui->btn_pharmacode_dir->setVisible( false );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsBarcode::NewResult(bool approved, const QString &value,  quint32 proc_id)
{
    if( pTool )
    {
        ui->le_extracted->setPlainText( pTool->GetExtractedText() );
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsBarcode::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    if( pTool )
    {
        ui->btn_angle->setChecked( pTool->GetLockAngle() );
        ui->btn_visible->setChecked( pTool->isVisible() );
        ui->le_name->setText( pTool->GetToolName() );
        ui->le_expected->setPlainText( pTool->GetExpectedText() );
        ui->le_extracted->setPlainText( pTool->GetExtractedText() );

        connect(pTool, SIGNAL(NewResult(bool,QString,quint32)), this, SLOT(NewResult(bool,QString,quint32)), Qt::UniqueConnection );
    }
}

/// ===========================================================================
///
/// ===========================================================================
bool ParamsBarcode::eventFilter(QObject *object, QEvent *event)
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

    if(object == ui->le_expected && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
           ui->le_expected->setPlainText(dlg.getText());
           if(pTool) pTool->SetExpectedText( dlg.getText() );
           return true;
        }
    }
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsBarcode::on_btn_visible_toggled(bool checked)
{
    if(pTool) pTool->setVisible( checked );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsBarcode::on_btn_angle_toggled(bool checked)
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
void ParamsBarcode::on_btn_show_result_toggled(bool checked)
{
    if( pTool )
    {
        pTool->ShowThresholded(checked);
        pTool->Exec(0);
        pTool->update();
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsBarcode::on_btn_pharmacode_toggled(bool checked)
{
    if( pTool )
    {
        pTool->SetPharmaCode(checked, ui->btn_pharmacode_dir->isChecked() );
        pTool->Exec(0);
        pTool->update();
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsBarcode::on_btn_pharmacode_dir_toggled(bool checked)
{

    if( checked )
        ui->btn_pharmacode_dir->setIcon( QIcon(":/icons/arrow-left_inverted.png") );
    else
        ui->btn_pharmacode_dir->setIcon( QIcon(":/icons/arrow-right.png") );

    if( pTool )
    {
        pTool->SetPharmaCode(ui->btn_pharmacode->isChecked(), checked );
        pTool->Exec(0);
        pTool->update();
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsBarcode::on_pushButton_clicked()
{
    if( ui->le_extracted->toPlainText().size() == 0 ) return;

    ui->le_expected->setPlainText( ui->le_extracted->toPlainText() );
    if(pTool)
    {
        pTool->SetExpectedText( ui->le_extracted->toPlainText() );
        pTool->Exec(0);
    }
}
