#include "formmainmenu.h"
#include "ui_form_mainmenuH.h"

#include "interface/windowmanager.h"
#include "util/sys_log.h"
#include "util/systemsettings.h"
#include "util/permission_check.h"
//#include "util/pdfmaker.h"

#ifdef _WIN32
#include <windows.h>
#endif

/// ===========================================================================
///
/// ===========================================================================
FormMainMenu::FormMainMenu(QWidget *parent) : QWidget(parent)
{
    ui = 0;
#ifdef _DISPLAY_HORIZONTAL_
    ui = new Ui::FormMainMenuH;
#else
    ui = new Ui::FormMainMenuV;
#endif
    ui->setupUi(this);

    ui->btn_products->setProperty(     "DEST", "PRODUCTS");
    ui->btn_config->setProperty(       "DEST", "CONFIG");
    ui->btn_alarms->setProperty(       "DEST", "ALARMS");
    ui->btn_printer->setProperty(      "DEST", "PRINTER");
    ui->btn_export_data->setProperty(  "DEST", "EXPORT");
    ui->btn_part11->setProperty(       "DEST", "PART11");
    ui->btn_version->setProperty(      "DEST", "VERSION");


    ui->frame_login->installEventFilter( this );

    ui->btn_part11->setVisible( qApp->property("USE_PART11").toBool() );


}

/// ===========================================================================
///
/// ===========================================================================
void FormMainMenu::showEvent(QShowEvent * event)
{
    Q_UNUSED(event);

    ui->frame_login->setVisible( WindowManager::instance()->GetCurrentUserLevel() == (-1) );
    ui->frame->setVisible( WindowManager::instance()->GetCurrentUserLevel()     != (-1) );

    WindowManager::instance()->SetInfoTop( "Menu Principal");
    CheckButtonPermissions(this);

    //ui->btn_part11->setVisible( qApp->property("USE_PART11").toBool() );
}

/// ===========================================================================
///
/// ===========================================================================
bool FormMainMenu::eventFilter(QObject* object, QEvent* event)
{
    if( event->type() == QEvent::MouseButtonRelease )
    {
        WindowManager::instance()->ShowScreen( "Login" );
    }
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void FormMainMenu::on_btn_products_clicked()
{
    WindowManager::instance()->ShowScreen("Products");
}

/// ===========================================================================
///
/// ===========================================================================
void FormMainMenu::on_btn_config_clicked()
{
    WindowManager::instance()->ShowScreen("Config");
}

/// ===========================================================================
///
/// ===========================================================================
void FormMainMenu::on_btn_alarms_clicked()
{
    WindowManager::instance()->ShowScreen("Alarms");
}

/// ===========================================================================
///
/// ===========================================================================
void FormMainMenu::on_btn_printer_clicked()
{
    if( P11(tr("Aceeso a impressora"), true ) == false ) return;
    WindowManager::instance()->ShowScreen("Printer");
}

/// ===========================================================================
///
/// ===========================================================================
void FormMainMenu::on_btn_export_data_clicked()
{
    WindowManager::instance()->ShowScreen("ExportData");
}

/// ===========================================================================
///
/// ===========================================================================
void FormMainMenu::on_btn_part11_clicked()
{
    WindowManager::instance()->ShowScreen("P11");
}

/// ===========================================================================
///
/// ===========================================================================
void FormMainMenu::on_btn_version_clicked()
{
    WindowManager::instance()->ShowScreen( "Version" );
}
