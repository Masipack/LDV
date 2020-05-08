#include "paramsfiducial.h"
#include "ui_paramsfiducial.h"

#include "mv/tools/mvfiducial.h"
#include "util/dlgkeyboard.h"

/// ===========================================================================
///
/// ===========================================================================
ParamsFiducial::ParamsFiducial(MvFiducial *p, QWidget *parent) : QWidget(parent),
    ui(new Ui::ParamsFiducial), pTool(p)
{
    ui->setupUi(this);
    ui->le_name->installEventFilter( this );


    if( pTool )
    {
        connect(pTool, SIGNAL(TemplateImage(QPixmap)), this, SLOT(TemplateImage(QPixmap)), Qt::UniqueConnection);
    }
}

/// ===========================================================================
///
/// ===========================================================================
ParamsFiducial::~ParamsFiducial()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    if( pTool )
    {
        connect(pTool, SIGNAL(TemplateImage(QPixmap)), this, SLOT(TemplateImage(QPixmap)), Qt::UniqueConnection);
        ui->btn_angle->setChecked( pTool->GetLockAngle() );
        ui->btn_visible->setChecked( pTool->isVisible() );
        ui->le_name->setText( pTool->GetToolName() );
    }
}


/// ===========================================================================
///
/// ===========================================================================
bool ParamsFiducial::eventFilter(QObject *object, QEvent *event)
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
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::SetRunMode()
{
    ui->btn_angle->setVisible( false );
    //ui->frm_contrast->setVisible( false );
}


/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_visible_toggled(bool checked)
{
    if(pTool) pTool->setVisible( checked );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_angle_toggled(bool checked)
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
void ParamsFiducial::on_btn_minus_similarity_clicked()
{
    int v = ui->lbl_similarity->text().left( ui->lbl_similarity->text().size() - 1 ).toInt();
    if( v <= 30 ) return;

    v--;

    ui->lbl_similarity->setText(QString::number(v) + "%");

    if( pTool ) pTool->SetMinimumSimilarity( v );

}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_plus_similarity_clicked()
{
    int v = ui->lbl_similarity->text().left( ui->lbl_similarity->text().size() - 1 ).toInt();
    if( v > 98 ) return;

    v++;

    ui->lbl_similarity->setText(QString::number(v) + "%");

    if( pTool ) pTool->SetMinimumSimilarity( v );
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_plus_region_clicked()
{
    int v = ui->lbl_region->text().left( ui->lbl_region->text().size() - 1 ).toInt();
    if( v > 70) return;

    v++;

    ui->lbl_region->setText(QString::number(v) + "%");

    if( pTool ) pTool->SetMinimumRegion(v);
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_minus_region_clicked()
{
    int v = ui->lbl_region->text().left( ui->lbl_region->text().size() - 1 ).toInt();
    if( v <= 5) return;

    v--;
    ui->lbl_region->setText(QString::number(v) + "%");

    if( pTool ) pTool->SetMinimumRegion(-v);
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::on_btn_edit_toggled(bool checked)
{
    if( pTool ){

        pTool->SetExecOnMove(checked ? false:true);
        if(checked==false) pTool->ResetMove();
        pTool->SetLock(checked ? false:true);
        pTool->update();

    }
}

/// ===========================================================================
///
/// ===========================================================================
void ParamsFiducial::TemplateImage(const QPixmap &image)
{
     int radius_tl=10;
     int radius_tr=10;
     int radius_bl=10;
     int radius_br=10;

     Debug("")
     QRegion region(0, 0, ui->lbl_Template->width(),ui->lbl_Template->height(), QRegion::Rectangle);

         //-----------------------------  top left ------------------------------------//
         QRegion round (0, 0, 2*radius_tl, 2*radius_tl, QRegion::Ellipse);
         QRegion corner(0, 0, radius_tl, radius_tl, QRegion::Rectangle);

         region = region.subtracted(corner.subtracted(round));

         //----------------------------- top right ------------------------------------//
         round  = QRegion(ui->lbl_Template->width()-2*radius_tr, 0, 2*radius_tr, 2*radius_tr, QRegion::Ellipse);
         corner = QRegion(ui->lbl_Template->width()-radius_tr, 0, radius_tr, radius_tr, QRegion::Rectangle);

         region = region.subtracted(corner.subtracted(round));

         //---------------------------- bottom right ---------------------------------//
         round  = QRegion(ui->lbl_Template->width()-2*radius_br,  ui->lbl_Template->height()-2*radius_br, 2*radius_br, 2*radius_br, QRegion::Ellipse);
         corner = QRegion(ui->lbl_Template->width()-radius_br,  ui->lbl_Template->height()-radius_br, radius_br, radius_br, QRegion::Rectangle);

         region = region.subtracted(corner.subtracted(round));

         //---------------------------- bottom left ---------------------------------//
         round  = QRegion(0, ui->lbl_Template->height()-2*radius_bl, 2*radius_bl, 2*radius_bl, QRegion::Ellipse);
         corner = QRegion(0, ui->lbl_Template->height()-radius_bl, radius_bl, radius_bl, QRegion::Rectangle);

         region = region.subtracted(corner.subtracted(round));

     ui->lbl_Template->setPixmap(image);
     ui->lbl_Template->setMask(region);
     ui->lbl_Template->update();


}
