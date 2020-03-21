#include "dialogpart11.h"
#include "ui_dialogpart11.h"
#include "util/dlgkeyboard.h"

#include <QtSql>
#include "util/sys_log.h"
#include "util/systemsettings.h"

/// ===========================================================================
///
/// ===========================================================================
DialogPart11::DialogPart11(QWidget *parent) : QDialog(parent), ui(new Ui::DialogPart11)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);
    ui->setupUi(this);

    ui->le_name->setReadOnly( true );
    ui->le_password->installEventFilter( this );
    ui->le_comments->installEventFilter( this );
    ui->btn_ok->setEnabled( false );
}

/// ===========================================================================
///
/// ===========================================================================
DialogPart11::~DialogPart11()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void DialogPart11::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)
    QString user = qApp->property("CURRENT_USER").toString();
    ui->le_name->setText( user );
}

/// ===========================================================================
///
/// ===========================================================================
QString DialogPart11::GetUser()
{
    return ui->le_name->text();
}

/// ===========================================================================
///
/// ===========================================================================
QString DialogPart11::GetComment()
{
    return ui->le_comments->text();
}

/// ===========================================================================
///
/// ===========================================================================
bool DialogPart11::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->le_password && event->type() == QEvent::MouseButtonRelease)
    {
        DlgKeyboard dlg;
        dlg.setPasswordMode( true );
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_password->setText( dlg.getText() );
        }
    }

    if(object == ui->le_comments && event->type() == QEvent::MouseButtonRelease)
    {
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_comments->setText( dlg.getText() );
        }
    }

    if( ui->le_comments->text().isEmpty() || ui->le_name->text().isEmpty() || ui->le_password->text().isEmpty() )
        ui->btn_ok->setEnabled( false );
    else
        ui->btn_ok->setEnabled( true );

    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void DialogPart11::on_btn_ok_clicked()
{
    QString hash_num = QByteArray( ui->le_password->text().toLatin1() ).toBase64();
    QString str_query = QString("select password, level, login from users where login = '%1' and active = 1").arg(ui->le_name->text());

    QSqlQuery query(QString(), QSqlDatabase::database("SYSTEM"));
    query.exec( str_query );

    if( query.next() )
    {
        QString h = query.value(0).toString();
        if( h == hash_num )
        {
            this->accept();
            return;
        }
    }

    P11(tr("Usuário ou senha inválido:") + ui->le_name->text());
    ui->label->setText(tr("Usuário ou senha inválido"));
}

/// ===========================================================================
///
/// ===========================================================================
void DialogPart11::on_btn_cancel_clicked()
{
    this->reject();
}
