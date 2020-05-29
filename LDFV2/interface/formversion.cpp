#include "formversion.h"
#include "ui_form_versionH.h"
#include "ui_form_versionV.h"

#include "interface/windowmanager.h"
#include "util/sys_log.h"
#include "util/systemsettings.h"

/// ===========================================================================
///
/// ===========================================================================
FormVersion::FormVersion(QWidget *parent) : QWidget(parent)
{  
    ui = 0;
#ifdef _DISPLAY_HORIZONTAL_
    ui = new Ui::FormVersionH;
#else
    ui = new Ui::FormVersionV;
#endif
    ui->setupUi(this);
}

/// ===========================================================================
///
/// ===========================================================================
void FormVersion::on_btn_menu_clicked()
{
    WindowManager::instance()->ShowScreen( "MainMenu" );
}


/// ===========================================================================
///
/// ===========================================================================
void FormVersion::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)
    WindowManager::instance()->SetInfoTop(tr("Versão de Software"));

    QString s;
    GetConfig(s, "FABRIMA/PEDIDO"            , QString("0000"));
    ui->le_pedido->setText(s);

    GetConfig(s, "FABRIMA/MODELO"            , QString("LDF"));
    ui->le_model->setText(s);

    GetConfig(s, "FABRIMA/VERSAO"            , QString("2.0"));
    ui->le_version->setText(s);

    GetConfig(s, "FABRIMA/FIRMWARE"          , QString("?"));
    ui->le_firmware->setText(s);

    GetConfig(s, "FABRIMA/CAMERA"            , QString("?"));
    ui->le_camera->setText(s);

    ui->btn_menu->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);

}
