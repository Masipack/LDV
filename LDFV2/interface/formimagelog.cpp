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

    WindowManager::instance()->SetInfoTop( "Fila de Imagens" );

    ui->btn_menu->setEnabled(WindowManager::instance()->GetCurrentUserLevel() != (-1));
    ui->btn_next->setEnabled(WindowManager::instance()->GetCurrentUserLevel() != (-1));
    ui->btn_prev->setEnabled(WindowManager::instance()->GetCurrentUserLevel() != (-1));


    int pos = 0;
    QImage im;
    QString s;
    if( ImageQueue::instance()->GetAt(pos, im, s) == false ) return;
    ui->lbl_image->setPixmap(QPixmap::fromImage(im));
    ui->lbl_pos->setText( QString::number( pos) );


}

void FormImageLog::on_btn_prev_clicked()
{
    int pos = ui->lbl_pos->text().toInt();
    if( pos == 0 ) return;

    pos--;

    QImage im;
    QString s;
    if( ImageQueue::instance()->GetAt(pos, im, s) == false ) return;
    ui->lbl_image->setPixmap(QPixmap::fromImage(im));
    ui->lbl_pos->setText( QString::number( pos) );
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

    ui->lbl_image->setPixmap(QPixmap::fromImage(im));
    ui->lbl_pos->setText( QString::number( pos) );
}


///===========================================================================
///
/// ===========================================================================
void FormImageLog::on_btn_menu_clicked()
{
    WindowManager::instance()->ShowLastScreen();
}


