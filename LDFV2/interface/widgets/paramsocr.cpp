#include "paramsocr.h"
#include "ui_paramsocr.h"

#include "mv/tools/mvocr.h"
#include "util/dlgkeyboard.h"
#include "util/systemsettings.h"
#include "util/alarmmanager.h"
#include "util/sys_log.h"

/// ===========================================================================
///
/// ===========================================================================
ParamsOCR::ParamsOCR(MvOCR *p, QWidget *parent) :  QWidget(parent),
    ui(new Ui::ParamsOCR), pTool(p)
{
    ui->setupUi(this);

    ui->le_name->installEventFilter( this );
    ui->le_expected->installEventFilter( this );

    white_filter = 0;

    ui->lbl_database->setVisible(qApp->property("USE_DATABASE").toBool());
    ui->cb_database->setVisible(qApp->property("USE_DATABASE").toBool());
}

/// ===========================================================================
///
/// ===========================================================================
ParamsOCR::~ParamsOCR()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR:: SetTableDataBase(const QList<QString> attributes)
{
    ui->cb_database->addItems(attributes);
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::SetExpectedText(const QString text)
{
    ui->le_expected->setPlainText(text);
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::SetBackgroundStateTool(const int &v)
{
  //  ui->btn_black_white->setChecked( v == 1 ? true:false );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::SetRotationStateTool(const bool &v)
{
    ui->btn_angle->setChecked(v);
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::NewResult(bool approved, const QString &value, quint32 proc_id)
{
    if( pTool )
    {
        ui->le_extracted->setPlainText( pTool->GetExtractedText() );
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::NewResultAttributes(const QString &value)
{
    ui->cb_database->setCurrentText(value);
    ui->le_expected->setPlainText(value);
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    if( pTool )
    {
        ui->btn_angle->setChecked( pTool->GetLockAngle() );
        ui->btn_visible->setChecked( pTool->isVisible() );
        ui->le_name->setText( pTool->GetToolName() );
        ui->le_expected->setPlainText( pTool->GetExpectedText() );
        ui->le_extracted->setPlainText( pTool->GetExtractedText() );
        white_filter=pTool->GetWhiteFilterSize();
        ui->btn_edit_position->setChecked(false);
        ui->btn_black_white->setChecked(pTool->GetBlackAndWhite()== 1 ? true:false);
        connect(pTool, SIGNAL(NewResult(bool,QString,quint32)), this, SLOT(NewResult(bool,QString,quint32)), Qt::UniqueConnection );
    }

    ui->lbl_white->setText( QString::number( white_filter ) );


}

/// ===========================================================================
///
/// ===========================================================================
bool ParamsOCR::eventFilter(QObject *object, QEvent *event)
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
               pTool->update();
           }
           return true;
        }
    }
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::on_btn_visible_toggled(bool checked)
{
    if(pTool) pTool->setVisible( checked );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::on_btn_angle_toggled(bool checked)
{
    if( pTool )
    {
        if( checked ) pTool->setRotation(0);
        pTool->SetLockAngle( checked );
      //  pTool->ReconfigPosition();
        pTool->Exec(0);
        pTool->update();

        ui->btn_angle->setText(checked? "Ângulo Fixo":"Ângulo Variável");
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::on_btn_minus_white_clicked()
{
    if( white_filter == 0 ) return;
    white_filter--;
    ui->lbl_white->setText( QString::number( white_filter ) ); //filterNames[white_filter] );

    if( pTool )
    {
        pTool->SetWhiteFilterSize( white_filter );
        pTool->Exec(0);
        pTool->update();
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::on_btn_plus_white_clicked()
{
    if( white_filter == 7 ) return;
    white_filter++;
    ui->lbl_white->setText( QString::number( white_filter ) ); //filterNames[white_filter] );

    if( pTool )
    {
        pTool->SetWhiteFilterSize( white_filter );
        pTool->Exec(0);
        pTool->update();
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::on_btn_show_result_toggled(bool checked)
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
void ParamsOCR::on_pushButton_clicked()
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
void ParamsOCR::on_cb_database_currentTextChanged(const QString &value)
{
    if(qApp->property("USE_DATABASE").toBool()==false) return;
    emit(NewAttribute(value));
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::on_btn_black_white_clicked(bool checked)
{

}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::on_btn_edit_position_toggled(bool checked)
{
    if( pTool ){

       // Debug(pTool->GetConfigPos())

        pTool->SetExecOnMove(checked ? false:true);
        pTool->SetResizeOnCenter(false);
        if(checked==false) pTool->ResetMove();
        pTool->SetLock(checked ? false:true);
       // Debug(pTool->GetConfigPos())
        pTool->ReconfigPosition();
        pTool->Exec(0);
      //  Debug(pTool->GetConfigPos())


        LOG(LOG_INFO_TYPE, "Editando a posição da ferramenta" );
        AlarmManager::instance()->SetAlarm(ALM_EDIT_POSITION);

    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsOCR::on_btn_black_white_toggled(bool checked)
{
    if(pTool)
    {
        pTool->SetBlackAndWhite(checked ? 1: -1);
        pTool->Exec(0);
        ui->btn_black_white->setText(checked ? "Fundo escuro": "Fundo claro");
    }

}
