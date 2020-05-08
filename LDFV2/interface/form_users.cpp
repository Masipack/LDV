#include "form_users.h"
#include "ui_form_users.h"

#include <QtSql>

#include "interface/windowmanager.h"
#include "util/sys_log.h"
#include "util/dlginfo.h"
#include "interface/form_newuser.h"
#include "util/permission_check.h"

/// ===========================================================================
///
/// ===========================================================================
FormUsers::FormUsers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormUsers)
{
    ui->setupUi(this);

    str_query_active_only = "select login, name, level, active from users WHERE active = 1 ORDER BY active DESC, login ASC";
    str_query_all         = "select login, name, level, active from users ORDER BY active DESC, login ASC";

    model.setQuery(str_query_active_only, QSqlDatabase::database("SYSTEM"));
    ui->tableView->setModel(&model);

    ui->tableView->horizontalHeader()->setStretchLastSection( true );
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setDefaultSectionSize(48);
    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(2, 200);

    model.setHeaderData(0, Qt::Horizontal, QObject::tr("Login"));
    model.setHeaderData(1, Qt::Horizontal, QObject::tr("Nome"));
    model.setHeaderData(2, Qt::Horizontal, QObject::tr("Nível"));
    model.setHeaderData(3, Qt::Horizontal, QObject::tr("Ativo"));

    ui->btn_menu->setProperty("DEST"            , "ALL");
    ui->btn_new->setProperty("DEST"             , "User_New");
    ui->btn_edit->setProperty("DEST"            , "User_Edit");
    ui->btn_delete->setProperty("DEST"          , "User_Delete");
    ui->btn_show_inactive->setProperty("DEST"   , "User_ShowInactive");
}

/// ===========================================================================
///
/// ===========================================================================
FormUsers::~FormUsers()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsers::UpdateTable()
{
    model.setQuery(str_query_active_only,QSqlDatabase::database("SYSTEM") );
    if( ui->btn_show_inactive->isChecked() == true )
    {
        model.setQuery(str_query_all, QSqlDatabase::database("SYSTEM") );
    }
    ui->tableView->setModel(&model);
}


/// ===========================================================================
///
/// ===========================================================================
void FormUsers::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    if(WindowManager::instance()->GetCurrentUserLevel() == -1 )
    {
        WindowManager::instance()->ShowScreen("MainMenu");
        return;
    }

    WindowManager::instance()->SetInfoTop( "Usuários" );
    ui->btn_show_inactive->setChecked( false );
    CheckButtonPermissions(this);
    UpdateTable();
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsers::on_btn_menu_clicked()
{
    WindowManager::instance()->ShowLastScreen();
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsers::on_btn_new_clicked()
{
    FormNewUser* f = static_cast<FormNewUser*>(WindowManager::instance()->GetScreen("NewUser"));
    if( f )
    {
        f->SetNewUserMode();
    }

    if( P11(tr("Usuário: Criando um novo usuário"), true) == false ) return;
    WindowManager::instance()->ShowScreen("NewUser");
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsers::on_btn_edit_clicked()
{
    if( ui->tableView->currentIndex().row() == -1 )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Selecione um usuário"), false, false );
        dlg.exec();
        return;
    }

    QModelIndex mi = ui->tableView->currentIndex();

    if( mi.row() == -1 )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Selecione um usuário"), false, false );
        dlg.exec();
        return;
    }

    FormNewUser* f = static_cast<FormNewUser*>(WindowManager::instance()->GetScreen("NewUser"));
    if( f )
    {
        f->SetDialogData(model, mi);
        if( P11(tr("Usuário: Alterando usuário"), true) == false ) return;
        WindowManager::instance()->ShowScreen( "NewUser" );
    }
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsers::on_btn_delete_clicked()
{
    QModelIndex mi = ui->tableView->currentIndex();

    if( mi.row() == -1 )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Selecione um usuário"), false, false );
        dlg.exec();
        return;
    }

    QString u_name  = model.data( mi.sibling(mi.row(),1) ).toString();
    QString u_login = model.data( mi.sibling(mi.row(),0) ).toString();

    if( P11(tr("Usuário: Apagando usuário"), true) == false ) return;

    DlgInfo dlg;
    dlg.SetMessage(DlgInfo::IT_QUESTION, tr("Deseja excluir usuário %1 ( %2 ) ?").arg( u_name ).arg( u_login), true, true );
    dlg.exec();
    if( dlg.result() == QDialog::Rejected ) return;

    QSqlQuery query( QString() , QSqlDatabase::database("SYSTEM") );
    if( query.exec( QString("UPDATE users SET active = 0 where login = '%1'").arg(u_login) ) == false )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Erro ao executar operação COD:%1\n%2").arg(__LINE__).arg(query.lastError().text()), false, false );
        dlg.exec();
        return;
    }
    UpdateTable();
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsers::on_btn_show_inactive_clicked()
{
    UpdateTable();
}
