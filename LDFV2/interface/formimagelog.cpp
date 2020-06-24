#include "formimagelog.h"
#include "ui_formimagelog.h"
#include "util/imagequeue.h"
#include "interface/windowmanager.h"


///===========================================================================
///
/// ===========================================================================
FormImageLog::FormImageLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormImageLog)
{
    ui->setupUi(this);
    ui->imagedevice->setScene( &IBImagelog );
}

///===========================================================================
///
/// ===========================================================================
FormImageLog::~FormImageLog()
{
    delete ui;
}


///===========================================================================
///
/// ===========================================================================
void FormImageLog::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    WindowManager::instance()->SetInfoTop( tr("Fila de Imagens"));

    ui->btn_menu->setEnabled(WindowManager::instance()->GetCurrentUserLevel() != (-1));
    ui->btn_next->setEnabled(WindowManager::instance()->GetCurrentUserLevel() != (-1));
    ui->btn_prev->setEnabled(WindowManager::instance()->GetCurrentUserLevel() != (-1));

    int pos = 0;
    QImage im;
    QString s;

    if( ImageQueue::instance()->GetAt(pos, im, s) == false ) return;

    ui->lbl_pos->setText( QString::number( pos) );

    ui->btn_next->setEnabled(true);

    IBImagelog.NewImage( im.rgbSwapped() );

    ui->textEdit->setText( s );


    ui->btn_menu->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_next->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_prev->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_zoom_fit->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_zoom_in->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_zoom_out->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_clear->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);




}

///===========================================================================
///
/// ===========================================================================
void FormImageLog::on_btn_prev_clicked()
{
    int pos = ui->lbl_pos->text().toInt();
    if( pos == 0 ) return;

    pos--;

    QImage im;
    QString s;
    if( ImageQueue::instance()->GetAt(pos, im, s) == false ) return;

    IBImagelog.NewImage( im.rgbSwapped() );
    ui->lbl_pos->setText( QString::number( pos) );
    ui->textEdit->setText( s );
}


///===========================================================================
///
/// ===========================================================================
void FormImageLog::on_btn_next_clicked()
{
    int pos = ui->lbl_pos->text().toInt();
    pos++;

    QImage im;
    QString s;
    if( ImageQueue::instance()->GetAt(pos, im, s) == false ) return;
    IBImagelog.NewImage( im.rgbSwapped() );
    ui->lbl_pos->setText( QString::number( pos) );
    ui->textEdit->setText( s );
}

///===========================================================================
///
/// ===========================================================================
void FormImageLog::on_btn_menu_clicked()
{
    WindowManager::instance()->ShowLastScreen();
}

///===========================================================================
///
/// ===========================================================================
void FormImageLog::on_btn_clear_clicked()
{
    IBImagelog.Reset();
    ImageQueue::instance()->Clear();
}
