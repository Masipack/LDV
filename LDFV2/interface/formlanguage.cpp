#include "formlanguage.h"
#include "ui_formlanguage.h"
#include "util/systemsettings.h"
#include "interface/windowmanager.h"

///===========================================================================
///
/// ===========================================================================
FormLanguage::FormLanguage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLanguage)
{
    ui->setupUi(this);
}

///===========================================================================
///
/// ===========================================================================
FormLanguage::~FormLanguage()
{
    delete ui;
}

///===========================================================================
///
/// ===========================================================================
void FormLanguage::on_btn_return_clicked()
{
  WindowManager::instance()->ShowScreen("MainMenu");
}

///===========================================================================
///
/// ===========================================================================
void FormLanguage::on_btn_ENG_clicked()
{
  SystemSettings::instance()->writeValue("en-US",QString("/SYSTEM/LANGUAGE"));
}

///===========================================================================
///
/// ===========================================================================
void FormLanguage::on_btn_PT_clicked()
{
  SystemSettings::instance()->writeValue("pt-br",QString("/SYSTEM/LANGUAGE"));
}

///===========================================================================
///
/// ===========================================================================
void FormLanguage::on_btn_BN_clicked()
{
  SystemSettings::instance()->writeValue("bs",QString("/SYSTEM/LANGUAGE"));
}
