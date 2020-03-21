#include "dlginfo.h"
#include "ui_dlginfo.h"

/// ===========================================================================
///
/// ===========================================================================
DlgInfo::DlgInfo(QWidget *parent) : QDialog(parent), ui(new Ui::DlgInfo)
{
    ui->setupUi(this);
    setWindowFlags( Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint );
}

/// ===========================================================================
///
/// ===========================================================================
DlgInfo::~DlgInfo()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void DlgInfo::SetVisible(const bool value)
{
    ui->btn_ok->setVisible(value);
}

/// ===========================================================================
///
/// ===========================================================================
void DlgInfo::SetMessage(ICON_TYPE it, const QString& msg, bool bShowCancel, bool bYesNo)
{
    if( it == IT_ERROR )    ui->lbl_icon->setPixmap( QPixmap(":/icons/msg_error.png") );
    if( it == IT_WARNING )  ui->lbl_icon->setPixmap( QPixmap(":/icons/msg_warning.png") );
    if( it == IT_QUESTION ) ui->lbl_icon->setPixmap( QPixmap(":/icons/msg_question.png") );
    if( it == IT_INFO )     ui->lbl_icon->setPixmap( QPixmap(":/icons/msg_info.png") );

    if( bYesNo )
    {
        ui->btn_ok->setText( tr("Sim") );
        ui->btn_cancel->setText( tr("Não") );
    }
    else
    {
        ui->btn_ok->setText( tr("OK") );
        ui->btn_cancel->setText( tr("Cancelar") );
    }

    ui->lbl_message->setText( msg );
    ui->btn_cancel->setVisible( bShowCancel );
}

/// ===========================================================================
///
/// ===========================================================================
void DlgInfo::showEvent(QShowEvent *event)
{
    qApp->processEvents();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgInfo::on_btn_ok_clicked()
{
    this->accept();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgInfo::on_btn_cancel_clicked()
{
    this->reject();
}
