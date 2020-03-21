#include "formprinter.h"
#include "ui_formprinter.h"

#include "interface/windowmanager.h"
#include "util/systemsettings.h"

/// ===========================================================================
///
/// ===========================================================================
FormPrinter::FormPrinter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPrinter)
{
    ui->setupUi(this);
}

/// ===========================================================================
///
/// ===========================================================================
FormPrinter::~FormPrinter()
{
    delete ui;
}



/// ===========================================================================
///
/// ===========================================================================
void FormPrinter::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)
    WindowManager::instance()->SetInfoTop( "Impressora");

    QString url;
    GetConfig(url, "SYSTEM/PRINTER_URL", QString("") );
    ui->widget->setUrl(QUrl(url));
    ui->widget->update();
}

/// ===========================================================================
///
/// ===========================================================================
void FormPrinter::on_btn_menu_clicked()
{
    WindowManager::instance()->ShowLastScreen();
}
