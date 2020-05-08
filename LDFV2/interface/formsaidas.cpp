#include "formsaidas.h"
#include "ui_formsaidas.h"
#include "interface/windowmanager.h"
#include "util/serialcontrol.h"
/// ===========================================================================
///
/// ===========================================================================
FormSaidas::FormSaidas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSaidas)
{
    ui->setupUi(this);
}

/// ===========================================================================
///
/// ===========================================================================
FormSaidas::~FormSaidas()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormSaidas::on_btn_return_clicked()
{
    for (int current_index = 0; current_index < 15; ++current_index) {

         SerialControl::instance()->ToggleBitOFF(current_index);

    }

    WindowManager::instance()->ShowLastScreen();
}

/// ===========================================================================
///
/// ===========================================================================
void FormSaidas::showEvent(QShowEvent *event)
{
    WindowManager::instance()->SetInfoTop(tr("Gerenciar Saídas"));

    foreach (QWidget* p, widget_output)
    {
        ui->scrollAreaWidgetContents->layout()->removeWidget( p );
        delete p;
    }

    if(!widget_output.isEmpty()) widget_output.clear();


    for(int i = 0; i < 15; i++)
    {
        widgetOutPut* p = new widgetOutPut();
        p->SetNumberOut(i+1);
        ui->scrollAreaWidgetContents->layout()->addWidget( p );
        widget_output.push_back( p );
    }
}
