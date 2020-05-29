#include "formnewproductfinish.h"
#include "ui_formnewproductfinish.h"

#include "interface/windowmanager.h"
#include "util/dlgkeyboard.h"

/// ===========================================================================
///
/// ===========================================================================
FormNewProductFinish::FormNewProductFinish(QWidget *parent) : QWidget(parent),
    ui(new Ui::FormNewProductFinish)
{
    ui->setupUi(this);

    ui->le_prod_name->installEventFilter( this );
}

/// ===========================================================================
///
/// ===========================================================================
FormNewProductFinish::~FormNewProductFinish()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
QString FormNewProductFinish::GetName()
{
    return ui->le_prod_name->text();
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductFinish::showEvent(QShowEvent *event)
{
    WindowManager::instance()->SetInfoTop(tr("Nova Inspeção (Finalizar Receita)") );
    ui->btn_finish->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);


}

/// ===========================================================================
///
/// ===========================================================================
bool FormNewProductFinish::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->le_prod_name && event->type() == QEvent::MouseButtonRelease )
    {
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_prod_name->setText( dlg.getText() );
        }
        return false;
    }
    return QObject::eventFilter(object, event);
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductFinish::on_btn_finish_clicked()
{
    if( ui->le_prod_name->text().size() == 0 ) return;
    emit(Closed(ui->le_prod_name->text()));
    WindowManager::instance()->ShowScreen("Products");
}


