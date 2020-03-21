#include "formusbexport.h"
#include "ui_formusbexport.h"

#include <QStorageInfo>
#include "interface/windowmanager.h"
#include "util/fileutil.h"
#include "util/dlginfo.h"
#include "util/systemsettings.h"

/// ===========================================================================
///
/// ===========================================================================
FormUsbExport::FormUsbExport(QWidget *parent) : QWidget(parent),
    ui(new Ui::FormUsbExport)
{
    ui->setupUi(this);
    connect(&timerUpdateName, SIGNAL(timeout()), this, SLOT(UpdateDeviceName()));
}

/// ===========================================================================
///
/// ===========================================================================
FormUsbExport::~FormUsbExport()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsbExport::UpdateDeviceName()
{
    QList<QStorageInfo> info = QStorageInfo::mountedVolumes();
    for( int i = 0; i < info.size(); i++ )
    {
        if( info.at(i).rootPath().contains("/media/") )
        {
            if( usbPath != (info.at(i).rootPath() + "/SVF_" + strPedido) )
            {
                quint64 n   = info.at(i).bytesFree();
                QString sz  = file_size_human(n);

                ui->lbl_usb_device->setText(tr("Drive USB: ") + info.at(i).displayName() + " (" + sz + tr(" livres)"));
                usbPath = info.at(i).rootPath() + "/SVF_" + strPedido;

                ui->lst_usb->clear();
                ui->lst_usb->addItems( GetFileList(usbPath, "fmt") );
            }
            return;
        }
    }
    usbPath.clear();
    ui->lst_usb->clear();
    ui->lbl_usb_device->setText(tr("Insira Drive USB"));
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsbExport::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    if(WindowManager::instance()->GetCurrentUserLevel() == -1 )
    {
        WindowManager::instance()->ShowScreen("MainMenu");
        return;
    }

    WindowManager::instance()->SetInfoTop( tr("Imp. e Exportar Dados") );

    GetConfig(strPedido, "FABRIMA/PEDIDO", QString("0000") );

    ui->lst_local->clear();
    ui->lst_local->addItems( GetFileList("./data/formats/", "fmt") );

    UpdateDeviceName();

    timerUpdateName.start(2000);
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsbExport::on_btn_return_clicked()
{
    timerUpdateName.stop();
    WindowManager::instance()->ShowScreen("MainMenu");
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsbExport::on_btn_from_usb_clicked()
{
    if( usbPath.isEmpty() ) return;

    if( ui->lst_usb->currentRow() == -1 )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_INFO, tr("Selecione uma receita (USB) para importar"), false, false );
        dlg.exec();
        return;
    }

    QString dest = "./data/formats/" + ui->lst_usb->currentItem()->text() + ".fmt";

    if( QFile::exists(dest) )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_WARNING, tr("ATENÇÃO\nJá existe um arquivo com o mesmo\nnome no sistema.\n\n Deseja sobrescrever este arquivo?"), true, true );
        if( dlg.exec() == QDialog::Rejected ) return;
        if( QFile::remove(dest) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha excluindo arquivo local para copia USB"));
            return;
        }
    }

    QString src = usbPath + "/" + ui->lst_usb->currentItem()->text() + ".fmt";

    if( P11(tr("Importação USB de receita:") + ui->lst_usb->currentItem()->text(), true) == false ) return;

    if( QFile::copy(src, dest) == false )
    {
        LOG(LOG_ERROR_TYPE, tr("Falha copiando arquivo de drive USB para o sistema"));
    }

    LOG(LOG_INFO_TYPE, tr("Arquivo copiado USB->SISTEMA:") + src + " " + dest);

    ui->lst_local->clear();
    ui->lst_local->addItems( GetFileList("./data/formats/", "fmt") );
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsbExport::on_btn_to_usb_clicked()
{
    if( usbPath.isEmpty() ) return;

    if( ui->lst_local->currentRow() == -1 )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_INFO, tr("Selecione uma receita (Sistema) para exportar"), false, false );
        dlg.exec();
        return;
    }

    if( !QDir(usbPath).exists() )
    {
        if( QDir().mkdir(usbPath) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha criando pasta em drive usb"));
            return;
        }
    }

    QString dest = usbPath + "/" + ui->lst_local->currentItem()->text() + ".fmt";

    if( QFile::exists(dest) )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_WARNING, tr("Já existe um arquivo com o mesmo\nnome no drive USB.\n\n Deseja sobrescrever este arquivo?"), true, true );
        if( dlg.exec() == QDialog::Rejected ) return;
        if( QFile::remove(dest) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha excluindo arquivo USB"));
            return;
        }
    }

    QString src = "./data/formats/" + ui->lst_local->currentItem()->text() + ".fmt";

    if( P11(tr("Exportação USB de receita:") + ui->lst_local->currentItem()->text(), true) == false ) return;

    if( QFile::copy(src, dest) == false )
    {
        LOG(LOG_ERROR_TYPE, tr("Falha copiando arquivo para drive USB"));
    }

    LOG(LOG_INFO_TYPE, tr("Arquivo copiado SISTEMA->USB:") + src + " " + dest);

    ui->lst_usb->clear();
    ui->lst_usb->addItems( GetFileList(usbPath, "fmt") );
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsbExport::on_btn_config_from_usb_clicked()
{
    if( usbPath.isEmpty() ) return;

    QStringList dest_files;
    dest_files = GetFileList(usbPath + "/", "dat", false);
    dest_files << "front.pfs"; // TODO: LISTA DE CAMERAS
    dest_files.removeOne("system.dat"); // database file can't be removed

    if( P11(tr("Importação USB de configurações"), true) == false ) return;

    for( auto item : dest_files )
    {
        QString dest = "./data/" + item;

        if( QFile::exists(dest) )
        {
            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_WARNING, tr("Já existe um arquivo com o mesmo\nnome ") + item + tr(" no SISTEMA.\n\n Deseja sobrescrever este arquivo?\n>> ESTA OPERACAO PODE CORROMPER O SISTEMA <<"), true, true );

            if( dlg.exec() == QDialog::Rejected ) continue;

            if( QFile::remove(dest) == false )
            {
                LOG(LOG_ERROR_TYPE, tr("Falha excluindo arquivo (BKP) SISTEMA"));
                dlg.SetMessage(DlgInfo::IT_ERROR, tr("Falha copiando o arquivo ") + item, false, false );
                dlg.exec();
                return;
            }
        }

        QString src = usbPath + "/" + item;

        if( QFile::copy(src, dest) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha copiando arquivo ") + item + tr(" para SISTEMA"));
            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_ERROR, tr("Falha copiando o arquivo ") + item, false, false );
            dlg.exec();
            return;
        }

        LOG(LOG_INFO_TYPE, tr("Arquivo copiado USB->SISTEMA: ") + item);
    }
}

/// ===========================================================================
///
/// ===========================================================================
void FormUsbExport::on_btn_config_to_usb_clicked()
{
    if( usbPath.isEmpty() ) return;

    if( !QDir(usbPath).exists() )
    {
        if( QDir().mkdir(usbPath) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha criando pasta em drive usb (2)"));
            return;
        }
    }

    QStringList dest_files;
    dest_files = GetFileList("./data/", "dat", false);
    dest_files << "front.pfs" << "back.pfs";

    if( P11(tr("Exportação USB de configurações"), true) == false ) return;

    bool bOverwrite = false;
    for( auto item : dest_files )
    {
        QString dest = usbPath + "/" + item;

        if( QFile::exists(dest) )
        {
            if( bOverwrite == false )
            {
                DlgInfo dlg;
                dlg.SetMessage(DlgInfo::IT_WARNING, tr("Já existem configurações no drive USB.\nDeseja sobrescrever estes arquivos?"), true, true );
                if( dlg.exec() == QDialog::Rejected ) return;
            }

            bOverwrite = true; // next files will be overwritten automatically

            if( QFile::remove(dest) == false )
            {
                LOG(LOG_ERROR_TYPE, tr("Falha excluindo arquivo (BKP) USB"));

                DlgInfo dlg;
                dlg.SetMessage(DlgInfo::IT_ERROR, tr("Falha copiando o arquivo ") + item, false, false );
                dlg.exec();
                return;
            }
        }

        QString src = "./data/" + item;
        if( QFile::copy(src, dest) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha copiando arquivo ") + item + tr(" para drive USB"));
            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_ERROR, tr("Falha copiando o arquivo ") + item, false, false );
            dlg.exec();
            return;
        }

        LOG(LOG_INFO_TYPE, tr("Arquivo copiado SISTEMA->USB: ") + item);
    }

    DlgInfo dlg;
    QString pedido;
    GetConfig(pedido, "FABRIMA/PEDIDO", QString("0000") );
    dlg.SetMessage(DlgInfo::IT_INFO, tr("Arquivos transferidos com sucesso !\n\nPasta USB:  ") + "SVF_" + pedido, false,false);
    dlg.exec();

}

/// ===========================================================================
///
/// ===========================================================================
void FormUsbExport::on_btn_to_usb_all_clicked()
{
    if( usbPath.isEmpty() ) return;

    if( !QDir(usbPath).exists() )
    {
        if( QDir().mkdir(usbPath) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha criando pasta em drive usb"));
            return;
        }
    }

    if( P11(tr("Exportação USB de todas as receitas"), true) == false ) return;

    for( int i = 0; i < ui->lst_local->count(); i++)
    {
        QString dest = usbPath + "/" + ui->lst_local->item(i)->text() + ".fmt";

        if( QFile::exists(dest) )
        {
            DlgInfo dlg;
            dlg.SetMessage(DlgInfo::IT_WARNING, tr("Já existe um arquivo com o mesmo\nnome no drive USB.\n\n Deseja sobrescrever este arquivo?"), true, true );
            if( dlg.exec() == QDialog::Rejected ) return;
            if( QFile::remove(dest) == false )
            {
                LOG(LOG_ERROR_TYPE, tr("Falha excluindo arquivo USB"));
                continue;
            }
        }

        QString src = "./data/formats/" + ui->lst_local->item(i)->text() + ".fmt";

        P11(tr("Exportação USB de receita:") + ui->lst_local->item(i)->text(), false);

        if( QFile::copy(src, dest) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha copiando arquivo para drive USB"));
        }

        LOG(LOG_INFO_TYPE, tr("Arquivo copiado SISTEMA->USB:") + src + " " + dest);
    }

    ui->lst_usb->clear();
    ui->lst_usb->addItems( GetFileList(usbPath, "fmt") );
}
