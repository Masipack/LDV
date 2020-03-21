#include "form_newuser.h"
#include "ui_form_newuser.h"

#include "interface/windowmanager.h"
#include "util/sys_log.h"
#include "util/dlginfo.h"
#include "util/dlgkeyboard.h"

/// ===========================================================================
///
/// ===========================================================================
FormNewUser::FormNewUser(QWidget *parent) : QWidget(parent), ui(new Ui::FormNewUser)
{
    ui->setupUi(this);

    ui->le_login->installEventFilter( this );
    ui->le_password->installEventFilter( this );
    ui->le_password_confirm->installEventFilter( this );
    ui->le_name->installEventFilter( this );

    user_level_names << tr("OPERADOR") << tr("SUPERVISOR") << tr("ADMINISTRADOR");
}

/// ===========================================================================
///
/// ===========================================================================
FormNewUser::~FormNewUser()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
bool FormNewUser::SetDialogData(QSqlQueryModel& model, QModelIndex& mi)
{
    ui->le_login->setText( model.data(mi.sibling(mi.row(),0)).toString() );
    ui->le_password->setText("");
    ui->le_password_confirm->setText("");
    ui->le_name->setText( model.data(mi.sibling(mi.row(),1)).toString() );

    int level = model.data(mi.sibling(mi.row(),2)).toInt();

    if( level < 0 || level >= user_level_names.size() ) return false;
    current_user_level = level;
    if( user_level_names.size() > current_user_level ) ui->le_level->setText( user_level_names.at(current_user_level) );

    int active = model.data(mi.sibling(mi.row(),3)).toInt();
    ui->btn_active->setChecked( active == 1 ? true : false  );

    bEditMode = true;

    ui->btn_new->setText( tr("Gravar\nAlteração") );

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewUser::SetNewUserMode()
{
    bEditMode = false;
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewUser::ClearScreen()
{
    ui->le_level->clear();
    ui->le_login->clear();
    ui->le_name->clear();
    ui->le_password->clear();
    ui->le_password_confirm->clear();
    ui->btn_active->setChecked( true );

    if( user_level_names.isEmpty() == false ) ui->le_level->setText( user_level_names.first() );
    current_user_level = 0;
    bEditMode = false;
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewUser::showEvent(QShowEvent * event)
{
    if(WindowManager::instance()->GetCurrentUserLevel() == -1 )
    {
        WindowManager::instance()->ShowScreen("MainMenu");
        return;
    }

    Q_UNUSED(event)
    if( bEditMode )
    {
        WindowManager::instance()->SetInfoTop(tr("Editar Usuário"));
        ui->btn_new->setText( tr("Gravar\nAlterações") );
    }
    else
    {
        WindowManager::instance()->SetInfoTop(tr("Novo Usuário"));
        ui->btn_new->setText( tr("Adicionar\nUsuário") );
    }

    if( bEditMode == false ) ClearScreen();
}

/// ===========================================================================
///
/// ===========================================================================
bool FormNewUser::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->le_login && event->type() == QEvent::MouseButtonRelease && bEditMode == false)
    {
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_login->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_password && event->type() == QEvent::MouseButtonRelease)
    {
        DlgKeyboard dlg;
        dlg.setPasswordMode( true );
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_password->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_password_confirm && event->type() == QEvent::MouseButtonRelease)
    {
        DlgKeyboard dlg;
        dlg.setPasswordMode( true );
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_password_confirm->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_name && event->type() == QEvent::MouseButtonRelease)
    {
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_name->setText( dlg.getText() );
        }
        return false;
    }
    return QObject::eventFilter(object, event);
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewUser::on_btn_menu_clicked()
{
    ClearScreen();
    WindowManager::instance()->ShowLastScreen();
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewUser::on_btn_new_clicked()
{
    if( ui->le_login->text().isEmpty() )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Login inválido"), false, false );
        dlg.exec();
        return;
    }

    if( ui->le_password->text().size() < 4 )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Senha inválida !\nA senha deve ter 4 ou mais caracteres"), false, false );
        dlg.exec();
        return;
    }

    if( ui->le_password->text() !=  ui->le_password_confirm->text() )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Senha inválida !\nA senha digitada deve coincidir nos dois campos."), false, false );
        dlg.exec();
        return;
    }

    QSqlQuery query(QString(), QSqlDatabase::database("SYSTEM"));

    if( bEditMode == false )
    {
        QString s = QString("select count(*) from users where login = '%1'").arg( ui->le_login->text() );
        if( query.exec(s) == false )
        {
            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_ERROR, tr("Erro ao executar operação COD:%1").arg(__LINE__), false, false );
            dlg.exec();
            return;
        }

        if( query.next() == false )
        {
            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_ERROR, tr("Erro ao executar operação COD:%1").arg(__LINE__), false, false );
            dlg.exec();
            return;
        }

        if( query.value(0).toInt() == 1 )
        {
            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_ERROR, tr("Usuário já cadastrado."), false, false );
            dlg.exec();
            return;
        }
    }

    query.clear();
    if( bEditMode )
        query.prepare( QString("UPDATE users SET login = :login, password = :password, name = :name, level = :level, active = :active where login = '%1'").arg(ui->le_login->text()));
    else
        query.prepare("INSERT INTO users (login, password, name, level, active) VALUES (:login, :password, :name, :level, :active)");

    QString hash_num = QByteArray( ui->le_password->text().toLatin1() ).toBase64(); //QString::number( hash ).toLatin1().data() ).toBase64();

    query.bindValue(":login"            , ui->le_login->text() );
    query.bindValue(":password"         , hash_num );
    query.bindValue(":name"             , ui->le_name->text() );
    query.bindValue(":level"            , current_user_level);
    query.bindValue(":active"           , ui->btn_active->isChecked() ? 1 : 0 );

    bool result = query.exec(); // TODO: tratar o retorno de inserir novo usuario

    if( bEditMode == true )
    {
        WindowManager::instance()->ShowLastScreen();
        return;
    }

    ClearScreen();
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewUser::on_btn_dec_level_clicked()
{
    if( current_user_level == 0 ) return;
    current_user_level--;
    if( user_level_names.size() > current_user_level ) ui->le_level->setText( user_level_names.at(current_user_level) );

}

/// ===========================================================================
///
/// ===========================================================================
void FormNewUser::on_btn_inc_level_clicked()
{
    if( current_user_level == 2 ) return;
    current_user_level++;
    if( user_level_names.size() > current_user_level ) ui->le_level->setText( user_level_names.at(current_user_level) );
}
