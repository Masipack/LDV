#include "configdatabase.h"
#include "ui_configdatabase.h"
#include "interface/windowmanager.h"
#include "TO/databaseto.h"
#include "util/fileutil.h"
#include "util/dlginfo.h"
#include "util/dlgkeyboard.h"


/// ===========================================================================
///
/// ===========================================================================
ConfigDataBase::ConfigDataBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigDataBase)
{
    ui->setupUi(this);

    ui->le_url->installEventFilter( this );
    ui->le_name->installEventFilter( this );
    ui->le_user->installEventFilter( this );
    ui->le_password->installEventFilter( this );
    ui->le_port->installEventFilter( this );
    ui->le_table->installEventFilter( this );
    ui->le_parameter_search->installEventFilter( this );


}

/// ===========================================================================
///
/// ===========================================================================
ConfigDataBase::~ConfigDataBase()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void ConfigDataBase::on_btn_save_clicked()
{
       if(Notify(ui->le_url)      == true) return ;
       if(Notify(ui->le_name)     == true) return ;
       if(Notify(ui->le_user)     == true) return ;
       if(Notify(ui->le_password) == true) return ;
       if(Notify(ui->le_table)    == true) return ;
       if(Notify(ui->le_parameter_search) == true) return ;

       DATABASETO  databaseTO;

       databaseTO.url = ui->le_url->text();
       databaseTO.name = ui->le_name->text();
       databaseTO.user = ui->le_user->text();
       databaseTO.password = ui->le_password->text();
       databaseTO.table = ui->le_table->text();
       databaseTO.parameter_search= ui->le_parameter_search->text();
       databaseTO.port = ui->le_port->text();

       QString error;
       DlgInfo dlg;

       if (WriteGenericTO(databaseTO,"./data/database/ldfbd.bin",error)== false);
       {
         dlg.SetMessage(DlgInfo::IT_WARNING, "Não foi possível alterar os dados");
       }

       dlg.SetMessage(DlgInfo::IT_WARNING, "Dados alterados com sucesso");

       dlg.exec();

       WindowManager::instance()->ShowScreen( "MainMenu" );
}


/// ===========================================================================
///
/// ===========================================================================
bool ConfigDataBase::Notify(QObject* object)
{
    QLineEdit*  le = qobject_cast<QLineEdit*>(object);

    if(le->text().isEmpty() || le->text().isEmpty())
    {
        le->setStyleSheet("background-color: rgb(93, 93, 93);color: rgb(255, 255, 255);border-color: rgb(241, 3, 38);border-style: solid; border-width: 2px 2px;");
        return true;
    }
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void ConfigDataBase::showEvent(QShowEvent *event)
{
    WindowManager::instance()->SetInfoTop(tr("Configurando parâmetros de acesso ao banco de dados"));
    Clear();
}

/// ===========================================================================
///
/// ===========================================================================
bool ConfigDataBase::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->le_url && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_url->setText( dlg.getText() );
        }
        return false;
    }


    if(object == ui->le_name && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_name->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_user && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_user->setText( dlg.getText() );
        }
        return false;
    }


    if(object == ui->le_password && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_password->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_port && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_port->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_table && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_table->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_parameter_search && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;

        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_parameter_search->setText( dlg.getText() );
        }
        return false;
    }
    return QObject::eventFilter(object, event);
}



/// ===========================================================================
///
/// ===========================================================================
void ConfigDataBase::Clear()
{
      ui->le_url->clear();
      ui->le_name->clear();
      ui->le_user->clear();
      ui->le_password->clear();
      ui->le_port->clear();
      ui->le_table->clear();
      ui->le_parameter_search->clear();

}

/// ===========================================================================
///
/// ===========================================================================
void ConfigDataBase::on_btn_cancel_clicked()
{
     WindowManager::instance()->ShowScreen( "MainMenu" );
}
