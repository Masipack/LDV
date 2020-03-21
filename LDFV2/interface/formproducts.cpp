#include "formproducts.h"
#include "ui_form_productsH.h"
#include "ui_form_productsV.h"

#include "util/systemsettings.h"
#include "interface/windowmanager.h"
#include "util/sys_log.h"
#include "util/fileutil.h"
#include "util/dlginfo.h"
#include "util/dlginfoprocess.h"
#include "interface/formnewproduct.h"
#include "util/permission_check.h"
#include "util/dlgkeyboard.h"
#include "interface/formprocess.h"

/// ===========================================================================
///
/// ===========================================================================
FormProducts::FormProducts(QWidget *parent) : QWidget(parent)
{ 
    ui = 0;
#ifdef _DISPLAY_HORIZONTAL_
    ui = new Ui::FormProductsH;
#else
    ui = new Ui::FormProductsV;
#endif
    ui->setupUi(this);

    ui->btn_delete->setProperty("DEST", "DELETE_PRODUCT");
    ui->btn_new->setProperty("DEST", "NEW_PRODUCT");
    ui->btn_load->setProperty("DEST", "LOAD_PRODUCT");
    ui->btn_menu->setProperty("DEST", "ALL");
    ui->btn_up->setProperty("DEST", "ALL");
    ui->btn_down->setProperty("DEST", "ALL");

    ui->le_batch->installEventFilter(this);
}

/// ===========================================================================
///
/// ===========================================================================
void FormProducts::on_btn_menu_clicked()
{
    WindowManager::instance()->ShowScreen( "MainMenu" );
}

/// ===========================================================================
///
/// ===========================================================================
bool FormProducts::eventFilter(QObject* object, QEvent* event)
{

    if(object == ui->le_batch && event->type() == QEvent::MouseButtonRelease )
    {
        ui->le_batch->setText( "" );
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_batch->setText( dlg.getText() );
        }
        return false;
    }
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void FormProducts::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)
    WindowManager::instance()->SetInfoTop("Produtos");
    CheckButtonPermissions(this);

    ui->listWidget->clear();
    ui->listWidget->addItems( GetFileList("./data/formats/", "fmt") );

    ui->frm_batch->setVisible( qApp->property("USE_PART11").toBool() );
    ui->le_batch->clear();
}

/// ===========================================================================
///
/// ===========================================================================
void FormProducts::on_btn_up_clicked()
{
    if( ui->listWidget->currentRow() <= 0 ) return;
    int r = ui->listWidget->currentRow() - 1;
    ui->listWidget->setCurrentRow( r );
}

/// ===========================================================================
///
/// ===========================================================================
void FormProducts::on_btn_down_clicked()
{
    if( ui->listWidget->currentRow() >= (ui->listWidget->count() - 1) ) return;
    int r = ui->listWidget->currentRow() + 1;
    ui->listWidget->setCurrentRow( r );
}

/// ===========================================================================
///
/// ===========================================================================
void FormProducts::on_btn_delete_clicked()
{
    if( ui->listWidget->currentRow() == -1 ) return;

    DlgInfo dlg;
    dlg.SetMessage(DlgInfo::IT_QUESTION, "Deseja excluir '" + ui->listWidget->currentItem()->text() + "' ?", true, true );
    dlg.exec();

    if( dlg.result() == QDialog::Rejected ) return;

    if( P11(tr("Formato excluido:") + ui->listWidget->currentItem()->text(), true) == false ) return;

    QFile::remove( QString("./data/formats/" + ui->listWidget->currentItem()->text() + ".fmt") );
    ui->listWidget->clear();
    ui->listWidget->addItems( GetFileList("./data/formats/", "fmt") );
}

/// ===========================================================================
///
/// ===========================================================================
void FormProducts::on_btn_new_clicked()
{
    if( P11(tr("Iniciada criação de receita"), true ) == false ) return;
    WindowManager::instance()->ShowScreen( "NewProduct" );

    FormNewProduct* p = (FormNewProduct*)WindowManager::instance()->GetScreen( "NewProduct" );

    if( p )
    {
        p->Init();
    }
}

/// ===========================================================================
///
/// ===========================================================================
void FormProducts::on_btn_load_clicked()
{
    if( ui->frm_batch->isVisible() && ui->le_batch->text().isEmpty() )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, "Informe o número do lote !", false, false );
        dlg.exec();
        return;
    }

    qApp->setProperty("BATCH", ui->le_batch->text());

    if( ui->listWidget->currentRow() == -1 ) return;


    FormProcess* p = (FormProcess*)WindowManager::instance()->GetScreen( "Process" );
    if( p == 0 ) return;
   // p->StartProcess();
    p->Init( ui->listWidget->currentItem()->text() );
    WindowManager::instance()->ShowScreen("Process");



//    if( TYPE == 0X01 )
//    {
//        if( P11(tr("Receita carregada:") + ui->listWidget->currentItem()->text(), true ) == false ) return;
//        FormTabletProcess* p = (FormTabletProcess*)WindowManager::instance()->GetScreen( "TabletProcess" );
//        if( p == 0 ) return;
//        p->Init( ui->listWidget->currentItem()->text() );
//        WindowManager::instance()->ShowScreen("TabletProcess");
//    }

//    if( TYPE == 0X02 )
//    {
//        if( P11(tr("Receita carregada:") + ui->listWidget->currentItem()->text(), true ) == false ) return;
//        FormCapsuleProcess* p = (FormCapsuleProcess*)WindowManager::instance()->GetScreen( "CapsuleProcess" );
//        if( p == 0 ) return;
//        p->Init( ui->listWidget->currentItem()->text() );
//        WindowManager::instance()->ShowScreen("CapsuleProcess");
//    }

}
