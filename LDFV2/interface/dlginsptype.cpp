#include "dlginsptype.h"
#include "ui_dlginsptype.h"

#include "util/dlgkeyboard.h"
#include "util/systemsettings.h"

/// ===========================================================================
///
/// ===========================================================================
DlgInspType::DlgInspType(QWidget *parent) : QDialog(parent),  ui(new Ui::DlgInspType)
{
    ui->setupUi(this);
    setWindowFlags( Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint );

    ui->le_name->installEventFilter(this);

    bool user_DATAMATRIX;
    GetConfig(user_DATAMATRIX, "SYSTEM/USE_DATAMATRIX", false);
    ui->btn_dmtx->setVisible(user_DATAMATRIX);
}

/// ===========================================================================
///
/// ===========================================================================
DlgInspType::~DlgInspType()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
bool DlgInspType::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->le_name && event->type() == QEvent::MouseButtonRelease)
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_name->setText( dlg.getText() );
        }
    }

    return false;
}

/// ===========================================================================
///
/// ===========================================================================
QString DlgInspType::GetName()
{
    return ui->le_name->text();
}

/// ===========================================================================
///
/// ===========================================================================
MV_TYPE DlgInspType::GetType()
{
    if( ui->btn_fiducial->isChecked() )     return MV_FIDUCIAL;
    if( ui->btn_ocr->isChecked() )          return MV_OCR;
    if( ui->btn_barcode->isChecked() )      return MV_BARCODE;
    if( ui->btn_dmtx->isChecked() )         return MV_DATAMATRIX;

    return MV_TYPE_UNDEFINED;
}

/// ===========================================================================
///
/// ===========================================================================
void DlgInspType::on_btn_ok_clicked()
{
    if( ui->le_name->text().isEmpty() )
    {
        ui->le_name->setFocus();
        return;
    }

    this->accept();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgInspType::on_btn_cancel_clicked()
{
    this->reject();
}
