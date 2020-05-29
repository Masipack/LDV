#include "formconfiguser.h"
#include "ui_formconfiguser.h"
#include "interface/windowmanager.h"
#include "util/sys_log.h"


/// ===========================================================================
///
/// ===========================================================================
FormConfigUser::FormConfigUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormConfigUser)
{
    ui->setupUi(this);
}

/// ===========================================================================
///
/// ===========================================================================
FormConfigUser::~FormConfigUser()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfigUser::showEvent(QShowEvent *event)
{

    WindowManager::instance()->SetInfoTop(tr("Gerenciar parâmetros de usuário"));

    foreach (QWidget* p, widget_configuser)
    {
        ui->scrollAreaWidgetContents->layout()->removeWidget( p );
        delete p;
    }

    if(!widget_configuser.isEmpty()) widget_configuser.clear();


    WidgetUser* MAX_EXPIRATE_DATE = new WidgetUser();
    MAX_EXPIRATE_DATE->SetNameKey(tr("Senha expira em dias: "));
    MAX_EXPIRATE_DATE->SetKey("MAX_EXPIRATE_DATE");
    MAX_EXPIRATE_DATE->SetMin(30);
    MAX_EXPIRATE_DATE->SetMax(180);
    ui->scrollAreaWidgetContents->layout()->addWidget( MAX_EXPIRATE_DATE);
    widget_configuser.push_back( MAX_EXPIRATE_DATE );


    WidgetUser* MAX_PASSWORD_ERRORS = new WidgetUser();
    MAX_PASSWORD_ERRORS->SetNameKey(tr("Número de tentativas erradas :"));
    MAX_PASSWORD_ERRORS->SetKey("MAX_PASSWORD_ERRORS");
    MAX_PASSWORD_ERRORS->SetMin(1);
    MAX_PASSWORD_ERRORS->SetMax(20);
    ui->scrollAreaWidgetContents->layout()->addWidget( MAX_PASSWORD_ERRORS );
    widget_configuser.push_back( MAX_PASSWORD_ERRORS);

    WidgetUser* LOGOFF_MINUTES = new WidgetUser();
    LOGOFF_MINUTES->SetNameKey(tr("LOGOFF automático em minutos:"));
    LOGOFF_MINUTES->SetKey("LOGOFF_MINUTES");
    LOGOFF_MINUTES->SetMin(1);
    LOGOFF_MINUTES->SetMax(60);
    ui->scrollAreaWidgetContents->layout()->addWidget( LOGOFF_MINUTES );
    widget_configuser.push_back( LOGOFF_MINUTES);


    ui->btn_return->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
}


/// ===========================================================================
///
/// ===========================================================================
void FormConfigUser::on_btn_return_clicked()
{

    foreach ( WidgetUser* c, widget_configuser)
    {

        if(c->hasChange())
        {
            if( P11(tr("Configurando Parâmetros:Alterando parâmetro %1  valor %2 ").arg(c->GetNameKey()).arg(QString::number(c->GetKeyValue())), false) == false ) return;
            c->WriteToFile();

        }
    }

     WindowManager::instance()->ShowLastScreen();
}
