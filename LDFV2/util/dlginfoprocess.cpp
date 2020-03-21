#include "dlginfoprocess.h"
#include "ui_dlginfoprocess.h"
#include "QDebug"

/// ===========================================================================
///
/// ===========================================================================
dlgInfoProcess::dlgInfoProcess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgInfoProcess)
{
    ui->setupUi(this);
}

/// ===========================================================================
///
/// ===========================================================================
dlgInfoProcess::~dlgInfoProcess()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void dlgInfoProcess::InitProcess(const QString& name)
{

}

/// ===========================================================================
///
/// ===========================================================================
void dlgInfoProcess::SetMessage(dlgInfoProcess::ICON_TYPE it, const QString &msg)
{
    if( it == IT_ERROR )    ui->lbl_icon->setPixmap( QPixmap(":/icons/msg_error.png") );
    if( it == IT_WARNING )  ui->lbl_icon->setPixmap( QPixmap(":/icons/msg_warning.png") );
    if( it == IT_QUESTION ) ui->lbl_icon->setPixmap( QPixmap(":/icons/msg_question.png") );
    if( it == IT_INFO )     ui->lbl_icon->setPixmap( QPixmap(":/icons/msg_info.png") );

    ui->lbl_message->setText( msg );

}

/// ===========================================================================
///
/// ===========================================================================
void dlgInfoProcess::Close()
{
    qDebug() << Q_FUNC_INFO;
    this->Close();
}

/// ===========================================================================
///
/// ===========================================================================
void dlgInfoProcess::showEvent(QShowEvent *event)
{
   SetMessage(dlgInfoProcess::IT_WARNING, " Aguarde carregando base de dados..... !");
}
