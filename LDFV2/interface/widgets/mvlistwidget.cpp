#include "mvlistwidget.h"
#include "ui_mvlistwidget.h"

#include <QScrollBar>


/// ===========================================================================
///
/// ===========================================================================
MVListWidget::MVListWidget(QString name, MV_TYPE type, QWidget *parent) : QWidget(parent),
    ui(new Ui::MVListWidget)
{
    ui->setupUi(this);

    ui->lbl_name->setText( name );

    switch(type)
    {
    case(MV_FIDUCIAL):
        ui->lbl_type->setPixmap( QPixmap(":/icons/mv_fiducial.png") );
    break;
    case(MV_OCR):
        ui->lbl_type->setPixmap( QPixmap(":/icons/mv_ocr.png") );
    break;
    case(MV_BARCODE):
        ui->lbl_type->setPixmap( QPixmap(":/icons/mv_barcode.png") );
    break;
    case(MV_DATAMATRIX):
        ui->lbl_type->setPixmap( QPixmap(":/icons/mv_dmtx.png") );
    break;
    default:
        ui->lbl_type->setPixmap( QPixmap("") );
    break;
    }

    my_type = type;
    pTool   = nullptr;


    setFocusPolicy(Qt::NoFocus);
}

/// ===========================================================================
///
/// ===========================================================================
MVListWidget::~MVListWidget()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void MVListWidget::SetTool(MvAbstractTool *p)
{
    pTool = p;
    connect(pTool, SIGNAL(NameChanged(QString))     , ui->lbl_name, SLOT(setText(QString))  , Qt::UniqueConnection );
    connect(pTool, SIGNAL(NewResult(bool, QString, quint32)) , this, SLOT(NewResult(bool,QString,quint32))   , Qt::UniqueConnection );
}

/// ===========================================================================
///
/// ===========================================================================
QString MVListWidget::GetName()
{
    return ui->lbl_name->text();
}

/// ===========================================================================
///
/// ===========================================================================
void MVListWidget::NewResult(bool approved, const QString &value, quint32 proc_id)
{
    if( approved )
    {
        ui->lbl_result->setStyleSheet( "background-color: rgb(  0, 128, 0); color: rgb(255, 255, 255);");
    }
    else
    {
        ui->lbl_result->setStyleSheet( "background-color: rgb(128,   0, 0); color: rgb(255, 255, 255);");
    }
    ui->lbl_result->setPlainText( value );
}

/// ===========================================================================
///
/// ===========================================================================
void MVListWidget::on_btn_up_clicked()
{
    int v  = ui->lbl_result->verticalScrollBar()->value();
    int mx = ui->lbl_result->verticalScrollBar()->maximum();
    int mn = ui->lbl_result->verticalScrollBar()->minimum();

    if( (v-1) < mn )
    {
        ui->lbl_result->verticalScrollBar()->setValue( mn );
        return;
    }
    ui->lbl_result->verticalScrollBar()->setValue( v-1 );
}

/// ===========================================================================
///
/// ===========================================================================
void MVListWidget::on_btn_dn_clicked()
{
    int v  = ui->lbl_result->verticalScrollBar()->value();
    int mx = ui->lbl_result->verticalScrollBar()->maximum();
    int mn = ui->lbl_result->verticalScrollBar()->minimum();

    if( (v+1) > mx )
    {
        ui->lbl_result->verticalScrollBar()->setValue( mx );
        return;
    }
    ui->lbl_result->verticalScrollBar()->setValue( v+1 );
}
