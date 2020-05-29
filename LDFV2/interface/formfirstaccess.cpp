#include "formfirstaccess.h"
#include "ui_formfirstaccess.h"
#include "util/fileutil.h"
#include "util/dlginfo.h"
#include "interface/windowmanager.h"
#include <QSqlQuery>
#include "util/dlgkeyboard.h"





/// ===========================================================================
///
/// ===========================================================================
FormFirstAccess::FormFirstAccess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFirstAccess)
{
    ui->setupUi(this);
    ui->le_password->installEventFilter(this);
    ui->le_password_confirm->installEventFilter(this);

}

/// ===========================================================================
///
/// ===========================================================================
FormFirstAccess::~FormFirstAccess()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormFirstAccess::showEvent(QShowEvent *event)
{

    ui->le_password_confirm->clear();
    ui->le_password->clear();

    ui->btn_save->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);

}

/// ===========================================================================
///
/// ===========================================================================
void FormFirstAccess::on_btn_save_clicked()
{

    if( ui->le_password->text().isEmpty() )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Login inválido"), false, false );
        dlg.exec();
        return;
    }

    if(!CheckPassword(ui->le_password->text()))
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Senha inválida :\nA senha deve ter conter  6 ou mais caracteres, maiuscula, minuscula e caracteres especial"), false, false );
        dlg.exec();
        return;
    }

    if(CheckEqualPassWord(ui->le_password->text()))
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("A nova senha não pode ser igual à anterior"), false, false );
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

    QString _name;
    int     _level;
    int     _active;

    QSqlQuery query(QString(), QSqlDatabase::database("SYSTEM"));

    QString user = qApp->property("CURRENT_USER").toString();

    QString str_query_user = QString("select login, name,level, active from users where login = '%1'").arg(user);

    query.exec( str_query_user );


    if( query.next() )
    {
        _name   = query.value(0).toString();
        _level  = query.value(1).toInt();
        _active = query.value(2).toInt();

    }

    query.clear();

    query.prepare( QString("UPDATE users SET login = :login, password = :password, name = :name, level = :level, active = :active"
                           ",first_access =:first_access,expiration_date =:expiration_date where login = '%1'").arg(user));

    QString hash_num = QByteArray( ui->le_password->text().toLatin1() ).toBase64(); //QString::number( hash ).toLatin1().data() ).toBase64();

    query.bindValue(":login"            , user);
    query.bindValue(":password"         , hash_num );
    query.bindValue(":name"             , _name );
    query.bindValue(":level"            , _level);
    query.bindValue(":active"           , 1);
    query.bindValue(":first_access"     , 0);
    query.bindValue(":expiration_date"    , QDateTime::currentDateTime());

    bool result = query.exec(); // TODO: tratar o retorno de inserir novo usuario

    DlgInfo dlg;
    dlg.SetMessage(result ? DlgInfo::IT_INFO : DlgInfo::IT_ERROR, tr(result ?"Alteração realizada com sucesso" :"Não foi possível realizar alteração"), false, false );
    dlg.exec();

    WindowManager::instance()->ShowScreen( "MainMenu" );

    return;
}

/// ===========================================================================
///
/// ===========================================================================
bool FormFirstAccess::eventFilter(QObject* object, QEvent* event)
{

    if(object == ui->le_password && event->type() == QEvent::MouseButtonRelease )
    {

        DlgKeyboard dlg;
        dlg.setPasswordMode( true );
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_password->setText( dlg.getText() );
        }
        return false;
    }


    if(object == ui->le_password_confirm && event->type() == QEvent::MouseButtonRelease )
    {

        DlgKeyboard dlg;
        dlg.setPasswordMode( true );
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_password_confirm->setText( dlg.getText() );
        }
        return false;
    }

    return false;

}
