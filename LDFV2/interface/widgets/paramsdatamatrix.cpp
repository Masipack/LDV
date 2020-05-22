#include "paramsdatamatrix.h"
#include "ui_paramsdatamatrix.h"
#include "mv/tools/mvdatamatrix.h"
#include "util/dlgkeyboard.h"
#include "util/sys_log.h"
#include "util/alarmmanager.h"

/// ===========================================================================
///
/// ===========================================================================
ParamsDataMatrix::ParamsDataMatrix(MvDataMatrix *p, QWidget *parent) :
    QWidget(parent), ui(new Ui::ParamsDataMatrix), pTool(p)
{

    ui->setupUi(this);
    ui->le_name->installEventFilter( this );
    ui->le_expected->installEventFilter( this );



}

/// ===========================================================================
///
/// ===========================================================================
ParamsDataMatrix::~ParamsDataMatrix()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsDataMatrix::SetRunMode()
{
    ui->btn_angle->setVisible( false );
    ui->frm_contrast->setVisible( false );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsDataMatrix::NewResult(bool approved, const QString &value, quint32 proc_id)
{
    if( pTool )
    {
      ui->le_extracted->setPlainText( pTool->GetExtractedText() );
    }
}


/// ===========================================================================
///
/// ===========================================================================
void ParamsDataMatrix::showEvent(QShowEvent * event)
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
bool ParamsDataMatrix::eventFilter(QObject *object, QEvent *event)
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
           if(pTool)
           {
               pTool->SetExpectedText( dlg.getText() );
               pTool->Exec(0);
           }


           return true;
        }
    }
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsDataMatrix::on_btn_visible_toggled(bool checked)
{
    if(pTool) pTool->setVisible( checked );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsDataMatrix::on_btn_angle_toggled(bool checked)
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
void ParamsDataMatrix::on_pushButton_clicked()
{
    if( ui->le_extracted->toPlainText().size() == 0 ) return;

    ui->le_expected->setPlainText( ui->le_extracted->toPlainText() );

    if(pTool)
    {
        pTool->SetExpectedText( ui->le_extracted->toPlainText() );
        pTool->Exec(0);
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsDataMatrix::on_btn_position_toggled(bool checked)
{
    if( pTool )
    {
        pTool->SetExecOnMove(checked ? false:true);
        pTool->SetLock(checked ? false:true);
        pTool->ReconfigPosition();
        pTool->update();

        LOG(LOG_INFO_TYPE, "Editando a posição da ferramenta" );
        AlarmManager::instance()->SetAlarm(ALM_EDIT_POSITION);

    }
}
