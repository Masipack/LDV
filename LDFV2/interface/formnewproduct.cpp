#include "formnewproduct.h"
#include "ui_formnewproduct.h"

#include "interface/windowmanager.h"
#include "util/systemsettings.h"
#include "util/fileutil.h"
#include "util/sys_log.h"
#include "util/dlginfo.h"
#include "TO/multicamto.h"

/// ===========================================================================
///
/// ===========================================================================
FormNewProduct::FormNewProduct(QWidget *parent) : QWidget(parent),
    ui(new Ui::FormNewProduct)
{
    ui->setupUi(this);
    current_form = 0;

    ui->btn_printer->setVisible(qApp->property("USE_PRINTER").toBool());


}

/// ===========================================================================
///
/// ===========================================================================
FormNewProduct::~FormNewProduct()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProduct::Init()
{
    QString camText;
    GetConfig(camText, "CAMERAS/CAMERA_LIST", QString("CAM1") );
    QStringList camList( camText.split(',') );

    for(int i = 0; i < camList.size(); i++)
    {
        FormNewProductContent* p = new FormNewProductContent();
        p->SetCamera( camList.at(i), i );
        p->StartTableDataBase();
        p->StartKeepAlive();
        forms.push_back( p );

        if( i == 0 )
        {
            ui->widget_content->layout()->addWidget( p );
            current_form = 0;
        }
    }

    ui->btn_btn_prev->setEnabled(false);
    ui->btn_btn_prev->setText(QString());

    formFinish = new FormNewProductFinish();
    connect(formFinish, SIGNAL(Closed(QString)), this, SLOT(WriteConfig(QString)));

    ui->btn_next->setEnabled(true);
    if( current_form == forms.size()-1 )
        ui->btn_next->setText(tr("Finalizar\nReceita"));
    else
        ui->btn_next->setText(tr("Próxima\nCâmera"));
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProduct::DeInit()
{
    for(int i = 0; i < forms.size(); i++)
    {
        forms[i]->StopCamera();
        delete forms[i];
        forms[i] = nullptr;
    }
    forms.clear();

    if( formFinish )
    {
        delete formFinish;
        formFinish = nullptr;
    }
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProduct::WriteConfig(const QString &name)
{
    MultiCamTO MultiTO;

    MultiTO.NAME = formFinish->GetName();

    for(int i = 0; i < forms.size(); i++)
    {
        ProductTO _TO;
        forms[i]->GetTO(_TO);
        MultiTO.TO.push_back( _TO );
    }

    //qDebug() << MultiTO;

//  WriteTO( MultiTO, "./data/formats/" + name + ".fmt",  )

    QString err;

    if( WriteTO(MultiTO, name + ".fmt", err, true) == false )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_QUESTION, tr("Erro ao  processar arquivo "), false, true );
        dlg.exec();

        LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: FALHA AO CRIAR ARQUIVO DE RECEITA"));


        QFile::remove( QString("./data/formats/" + name + ".fmt") );

        DeInit();

        if( dlg.result() == QDialog::Accepted ) return;
     }

     DeInit();
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProduct::showEvent(QShowEvent *event)
{


}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProduct::on_btn_btn_prev_clicked()
{
    if( current_form == forms.size() )
    {
        ui->widget_content->layout()->removeWidget( formFinish );
        formFinish->close();
        current_form--;
        ui->widget_content->layout()->addWidget( forms.at(current_form) );
        forms.at(current_form)->show();

        ui->btn_next->setEnabled(true);
        ui->btn_next->setText(tr("Finalizar\nReceita"));

        return;
    }

    if( current_form == 0 )
    {
        return;
    }

    ui->widget_content->layout()->removeWidget( forms.at(current_form) );
    forms.at(current_form)->close();
    current_form--;
    ui->widget_content->layout()->addWidget( forms.at(current_form) );
    forms.at(current_form)->show();

    if( current_form != forms.size()-1 )  ui->btn_next->setText(tr("Próxima\nCâmera"));

    if( current_form == 0 )
    {
        ui->btn_btn_prev->setEnabled(false);
        ui->btn_btn_prev->setText(QString());
    }
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProduct::on_btn_next_clicked()
{
    if( current_form == forms.size() ) return;

    if( current_form == forms.size()-1 )
    {
        ui->widget_content->layout()->removeWidget( forms.at(current_form) );
        forms.at(current_form)->close();
        current_form++;
        ui->widget_content->layout()->addWidget( formFinish );
        formFinish->show();
        ui->btn_next->setEnabled(false);
        ui->btn_next->setText(QString());
        return;
    }

    ui->widget_content->layout()->removeWidget( forms.at(current_form) );
    forms.at(current_form)->close();
    current_form++;
    ui->widget_content->layout()->addWidget( forms.at(current_form) );
    forms.at(current_form)->show();

    if( current_form == forms.size()-1 )  ui->btn_next->setText(tr("Finalizar\nReceita"));

    ui->btn_btn_prev->setEnabled(true);
    ui->btn_btn_prev->setText(QString("Câmera\nAnterior"));
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProduct::on_btn_return_clicked()
{
    DeInit();
    WindowManager::instance()->ShowScreen("Products");
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProduct::on_btn_printer_clicked()
{
    WindowManager::instance()->ShowScreen("Printer");
}
