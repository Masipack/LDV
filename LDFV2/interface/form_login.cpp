#include "form_login.h"
#include "ui_form_login.h"


#include "interface/windowmanager.h"
#include "util/sys_log.h"
#include "util/dlgkeyboard.h"
#include "util/dlginfo.h"
#include "util/fileutil.h"
#include "util/systemsettings.h"
#include "interface/formfirstaccess.h"


/// ===========================================================================
///
/// ===========================================================================
FormLogin::FormLogin(QWidget *parent) : QWidget(parent), ui(new Ui::FormLogin)
{
    ui->setupUi(this);
    ui->le_user->installEventFilter(this);
    ui->le_password->installEventFilter(this);

    setMouseTracking(true);

    GetConfig(EXPIRATE_DATA, "SYSTEM/MAX_EXPIRATE_DATE", 90);
}

/// ===========================================================================
///
/// ===========================================================================
FormLogin::~FormLogin()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormLogin::Clear()
{
    ui->le_user->clear();
    ui->le_password->clear();
    ui->lbl_info->clear();
}

/// ===========================================================================
///
/// ===========================================================================
void FormLogin::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    WindowManager::instance()->SetInfoTop(tr("Login de Usuário"));

    ui->btn_return->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_logoff->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);


}

/// ===========================================================================
///
/// ===========================================================================
bool FormLogin::eventFilter(QObject* object, QEvent* event)
{


    if(object == ui->le_user && event->type() == QEvent::MouseButtonRelease )
    {

        ui->lbl_info->setText( "" );
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {          
            ui->le_user->setText( dlg.getText() );
            ui->le_user->update();

        }

        return false;
    }

    if(object == ui->le_password && event->type() == QEvent::MouseButtonRelease )
    {
        ui->lbl_info->setText( "" );
        DlgKeyboard dlg;
        dlg.setPasswordMode( true );
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_password->setText( dlg.getText() );
        }

        return false;
    }

    return QObject::eventFilter(object, event);

}

/// ===========================================================================
///
/// ===========================================================================
void FormLogin::on_btn_return_clicked()
{
    WindowManager::instance()->ShowLastScreen();
}

/// ===========================================================================
///
/// ===========================================================================
void FormLogin::on_btn_login_clicked()
{
    if( ui->le_user->text().isEmpty() || ui->le_password->text().isEmpty() )
    {
        ui->lbl_info->setText(tr("Campos Inválidos"));
        return;
    }

    // PARA "RECOVERY"
    if( ui->le_user->text() == "FMA" && ui->le_password->text() == "ATELMT" )
    {
        WindowManager::instance()->SetCurrentUser("FMA", 3);
        WindowManager::instance()->ShowLastScreen();
        return;
    }

    bool first_access = false ;

    QSqlQuery query(QString(), QSqlDatabase::database("SYSTEM"));

    QString str_query_active = QString("select active,first_access from users where login = '%1'").arg(ui->le_user->text());

    query.exec( str_query_active );

    if( query.next() )
    {

        first_access= query.value(1).toBool();

        if(!query.value(0).toBool())
        {
            ui->lbl_info->setText(tr("Usuário desativado "));
            return ;
        }

    }

    QString hash_num = QByteArray( ui->le_password->text().toLatin1() ).toBase64();

    QString str_query = QString("select password, level, login,first_access,expiration_date from users where login = '%1' and active = 1").arg(ui->le_user->text());

    query.exec(str_query);

    if( query.next() )
    {
        QString h = query.value(0).toString();


        if( h == hash_num )
        {
            if(query.value(3).toBool())
            {
                WindowManager::instance()->SetCurrentUser(query.value(2).toString(), query.value(1).toInt());

                P11(tr("Gerenciar Senha:Login de usuário primeiro acesso ") + query.value(2).toString());

                WindowManager::instance()->SetInfoTop(tr("Gerenciar Senha:Primeiro acesso usuário"));
                WindowManager::instance()->ShowScreen("FirstAccess");
                Clear();

                return;

            }else {

                if(CheckExpirate(query.value(4).toDateTime(),90))
                {

                    WindowManager::instance()->SetCurrentUser(query.value(2).toString(), query.value(1).toInt());

                    P11(tr("Gerenciar Senha:Senha usuário expirada") + query.value(2).toString());

                    WindowManager::instance()->SetInfoTop( tr("Gerenciar Senha:Senha expirada") );
                    WindowManager::instance()->ShowScreen("FirstAccess");

                    Clear();
                    return;

                }

                WindowManager::instance()->SetCurrentUser(query.value(2).toString(), query.value(1).toInt());

                P11(tr("Login de usuario:") + query.value(2).toString());

                WindowManager::instance()->ShowLastScreen();
                error_count[ui->le_user->text()] = 0;
                Clear();
                return;
            }

        }
    }

    int nmax;

    GetConfig(nmax, "SYSTEM/MAX_PASSWORD_ERRORS", 3);

    error_count[ui->le_user->text()]++;

    ui->lbl_info->setText(tr("Usuário ou senha inválido (%1)").arg( error_count.contains(ui->le_user->text()) ? nmax-error_count[ui->le_user->text()] : 3  ));

    P11(tr("Login de usuário INVALIDO:") + ui->le_user->text());

    ui->le_password->clear();


    if( error_count[ui->le_user->text()] == nmax )
    {
        QSqlQuery query( QString() , QSqlDatabase::database("SYSTEM") );
        if( query.exec( QString("UPDATE users SET active = 0 where login = '%1'").arg(ui->le_user->text()) ) == true )
        {
            P11(tr("Login de usuário DESATIVADO:") + ui->le_user->text());

            error_count[ui->le_user->text()] = 0;

            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_WARNING, tr("Usuário %1 desativado por excesso de tentativas de login (%2)").arg(ui->le_user->text()).arg(nmax), false, false );
            dlg.exec();

            if(P11(tr("Usuário %1 desativado por excesso de tentativas de login (%2)").arg(ui->le_user->text()).arg(nmax), true) == false ) return;

            if(first_access)  WindowManager::instance()->ShowScreen("FirstAccess");

            WindowManager::instance()->ShowLastScreen();

            Clear();
            return;
        }
    }
}

/// ===========================================================================
///
/// ===========================================================================
void FormLogin::on_btn_logoff_clicked()
{
    P11(tr("Logoff de usuario:") + WindowManager::instance()->GetCurrentUser());
    WindowManager::instance()->SetCurrentUser("SISTEMA", -1);
    WindowManager::instance()->ShowLastScreen();
}
