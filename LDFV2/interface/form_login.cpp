#include "form_login.h"
#include "ui_form_login.h"


#include "interface/windowmanager.h"
#include "util/sys_log.h"
#include "util/dlgkeyboard.h"
#include "util/dlginfo.h"
#include "util/fileutil.h"
#include "util/systemsettings.h"

/// ===========================================================================
///
/// ===========================================================================
FormLogin::FormLogin(QWidget *parent) : QWidget(parent), ui(new Ui::FormLogin)
{
    ui->setupUi(this);
    ui->le_user->installEventFilter(this);
    ui->le_password->installEventFilter(this);

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
void FormLogin::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)
    WindowManager::instance()->SetInfoTop( "Login de Usuário" );

    ui->le_user->clear();
    ui->le_password->clear();

    if( WindowManager::instance()->GetCurrentUserLevel() == -1 )
        ui->btn_logoff->setEnabled(false);
    else
        ui->btn_logoff->setEnabled(true);
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

    return false;

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
        WindowManager::instance()->SetCurrentUser("FMA", 2);
        WindowManager::instance()->ShowLastScreen();
        return;
    }

    QString hash_num = QByteArray( ui->le_password->text().toLatin1() ).toBase64();

    QString str_query = QString("select password, level, login from users where login = '%1' and active = 1").arg(ui->le_user->text());

    QSqlQuery query(QString(), QSqlDatabase::database("SYSTEM"));
    query.exec( str_query );

    if( query.next() )
    {
        QString h = query.value(0).toString();
        if( h == hash_num )
        {
            P11(tr("Login de usuario:") + query.value(2).toString());
            WindowManager::instance()->SetCurrentUser(query.value(2).toString(), query.value(1).toInt());
            WindowManager::instance()->ShowLastScreen();
            error_count[ui->le_user->text()] = 0;
            return;
        }
    }

    int nmax;
    GetConfig(nmax, "SYSTEM/MAX_PASSWORD_ERRORS", 3);

    error_count[ui->le_user->text()]++;

    ui->lbl_info->setText(tr("Usuário ou senha inválido (%1)").arg( error_count.contains(ui->le_user->text()) ? nmax-error_count[ui->le_user->text()] : 3  ));


    P11(tr("Login de usuario INVALIDO:") + ui->le_user->text());

    ui->le_password->clear();


    if( error_count[ui->le_user->text()] == nmax )
    {
        QSqlQuery query( QString() , QSqlDatabase::database("SYSTEM") );
        if( query.exec( QString("UPDATE users SET active = 0 where login = '%1'").arg(ui->le_user->text()) ) == true )
        {
            P11(tr("Login de usuario DESATIVADO:") + ui->le_user->text());

            error_count[ui->le_user->text()] = 0;

            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_WARNING, tr("Usuário %1 desativado por excesso de tentativas de login (%2)").arg(ui->le_user->text()).arg(nmax), false, false );
            dlg.exec();

            WindowManager::instance()->ShowLastScreen();

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
