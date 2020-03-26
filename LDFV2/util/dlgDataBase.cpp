#include "dlgDataBase.h"
#include "ui_dlgDataBase.h"
#include "util/dlgkeyboard.h"




/// ===========================================================================
///
/// ===========================================================================
DlgDataBase::DlgDataBase(QWidget *parent) : QDialog(parent), ui(new Ui::DlgDataBase)
{
    ui->setupUi(this);
    setWindowFlags( Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint );
    ui->le_lote->installEventFilter(this);
}

/// ===========================================================================
///
/// ===========================================================================
DlgDataBase::~DlgDataBase()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
QString DlgDataBase::GetLote()
{
    return ui->le_lote->text();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgDataBase::showEvent(QShowEvent *event)
{
    qApp->processEvents();
}

/// ===========================================================================
///
/// ===========================================================================
bool DlgDataBase::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->le_lote && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_lote->setText( dlg.getText() );
        }
        return false;
    }
}

/// ===========================================================================
///
/// ===========================================================================
void DlgDataBase::on_btn_ok_clicked()
{
    this->accept();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgDataBase::on_btn_cancel_clicked()
{
    this->reject();
}
