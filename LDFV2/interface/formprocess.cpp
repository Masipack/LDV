#include "formprocess.h"
#include "ui_formprocess.h"

#include "TO/multicamto.h"
#include "util/fileutil.h"
#include "interface/windowmanager.h"
#include "util/dlginfo.h"
#include "util/fileutil.h"
#include "util/systemsettings.h"


/// ===========================================================================
///
/// ===========================================================================
FormProcess::FormProcess(QWidget *parent) : QWidget(parent), ui(new Ui::FormProcess),use_PRINTER(false)
{
    ui->setupUi(this);

    b_save_without_msg = false;

   // connect(this, SIGNAL(Close()), &dlg, SLOT(Close()), Qt::QueuedConnection);

    dlg.SetMessage(DlgInfo::IT_WARNING, " Aguarde, carregando inspeção..... !");
    dlg.SetVisible(false);

    ui->btn_printer->setVisible(qApp->property("USE_PRINTER").toBool());

}


/// ===========================================================================
///
/// ===========================================================================
FormProcess::~FormProcess()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
bool FormProcess::Init(const QString &file_name)
{



    Name = file_name;

    QString err;
    
    if( ReadTO(to, file_name + ".fmt", err) == false )
    {
        LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1").arg(err));
        return false;
    }

    ui->btn_btn_prev->setVisible( (to.TO.size() > 1) );
    ui->btn_next->setVisible( (to.TO.size() > 1) );

    dlg.show();

    for (int var = 0; var < 100; ++var) {
        qApp->processEvents(QEventLoop::ProcessEventsFlag::AllEvents);
        QThread::msleep(1);
    }

    int number_output = 0;
    
    for(int i = 0; i < to.TO.size(); i++ )
    {
        FormProcessContent* p = new FormProcessContent();
        
        p->SetCamera( to.TO[i].CAM_NAME, i );
        p->SetTO( to.TO[i] );
        p->SetFileName(file_name);
        p->SetOutput(number_output++);

        connect(p, SIGNAL(WriteConfig(QString)), this, SLOT(WriteConfig(QString)),Qt::UniqueConnection);

        forms.push_back( p );

        if( i == 0 )
        {
            ui->widget_content->layout()->addWidget( p );
        }
        
                
    }
    
    current_form = 0;

    ui->btn_btn_prev->setEnabled(current_form > 0);
    ui->btn_next->setEnabled(current_form < forms.size() - 1 );

    dlg.close();

    return true;

}

/// ===========================================================================
///
/// ===========================================================================
void FormProcess::DeInit()
{
    for(int i = 0; i < forms.size(); i++)
    {
        forms[i]->StopCamera();
        delete forms[i];
        forms[i] = nullptr;
    }
    forms.clear();
}


/// ===========================================================================
///
/// ===========================================================================
void FormProcess::on_btn_btn_prev_clicked()
{
    if( current_form == 0 ) return;

    ui->widget_content->layout()->removeWidget( forms.at(current_form) );
    forms.at(current_form)->close();
    current_form--;
    ui->widget_content->layout()->addWidget( forms.at(current_form) );
    forms.at(current_form)->show();
    ui->btn_btn_prev->setEnabled(current_form > 0);
    ui->btn_next->setEnabled(  current_form < forms.size() - 1 );
}

/// ===========================================================================
///
/// ===========================================================================
void FormProcess::on_btn_next_clicked()
{
    if( current_form == forms.size()-1 ) return;

    ui->widget_content->layout()->removeWidget( forms.at(current_form) );
    forms.at(current_form)->close();
    current_form++;
    ui->widget_content->layout()->addWidget( forms.at(current_form) );
    forms.at(current_form)->show();
    ui->btn_btn_prev->setEnabled(current_form > 0);
    ui->btn_next->setEnabled(  current_form < forms.size() - 1 );
}

/// ===========================================================================
///
/// ===========================================================================
void FormProcess::on_btn_return_clicked()
{

    for(int i = 0; i < forms.size(); i++)
    {
       ProductTO _TO;

       if(forms[i])
       {
           forms[i]->GetTO(_TO);
           if(forms[i]->ChangesTOOLS(_TO))
           {
               b_save_without_msg = true;
               WriteConfig(Name);
               break;
           }
       }
    }

    DlgInfo dlg;
    dlg.SetMessage(DlgInfo::IT_QUESTION, tr("Deseja interromper a inspeção ?"), true, true );
    dlg.exec();
    if( dlg.result() == QDialog::Rejected ) return;

    if( P11(tr("Inspeção interrompida:") + Name, true) == false ) return;

    qApp->setProperty("BATCH", "");

    DeInit();

    b_save_without_msg= false;
    WindowManager::instance()->ShowScreen("Products");

}


/// ===========================================================================
///
/// ===========================================================================
void FormProcess::showEvent(QShowEvent *event)
{

}

/// ===========================================================================
///
/// ===========================================================================
void FormProcess::WriteConfig(const QString &name)
{
    MultiCamTO MultiTO;

    MultiTO.NAME = Name;

    for(int i = 0; i < forms.size(); i++)
    {
        ProductTO _TO;
        forms[i]->GetTO(_TO);
        MultiTO.TO.push_back( _TO );
    }

    DlgInfo dlg;
    dlg.SetMessage(DlgInfo::IT_QUESTION, tr("Gravar alterações na receita?"), true, true );
    dlg.exec();

    if( dlg.result() == QDialog::Rejected ) return;


    if( P11(tr("Atributos de inspeção alterados:") + Name, !b_save_without_msg?true:false) == false ) return;

    QString err;

    if( WriteTO(MultiTO, name + ".fmt", err, true) == false )
    {
        LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1").arg(err));
        return;
    }

}

/// ===========================================================================
///
/// ===========================================================================
void FormProcess::on_btn_printer_clicked()
{
     WindowManager::instance()->ShowScreen("Printer");
}
