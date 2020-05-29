#include "formnewproductcontent.h"
#include "ui_formnewproductcontent.h"

#include "interface/windowmanager.h"
#include "interface/widgets/mvlistwidget.h"
#include "interface/dlginsptype.h"
#include "util/systemsettings.h"
#include "util/fileutil.h"
#include "mv/camera/cameramanager.h"
#include "util/serialcontrol.h"
#include "util/msqlsingleton.h"
#include "util/dlgDataBase.h"
#include "util/dlginfo.h"

/// ===========================================================================
///
/// ===========================================================================
FormNewProductContent::FormNewProductContent(QWidget *parent) : QWidget(parent),
    ui(new Ui::FormNewProductContent)
{
    ui->setupUi(this);
    tmColor.setInterval(150);
    index_color         = 0;
    index_color_dir     = 13;

    connect(&tmColor, SIGNAL(timeout()), this, SLOT(TimerColor()));

    QList<int> list;
    list << 529 << 124;
    ui->splitter->setSizes(list);

    ui->imagedevice->setScene(&ib);

    ui->btn_SincronizeDataBase->setVisible(qApp->property("USE_DATABASE").toBool());

}

/// ===========================================================================
///
/// ===========================================================================

FormNewProductContent::~FormNewProductContent()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::SetCamera(const QString &name, int n)
{
    camName = name;
    camNum = n;

    MvCamera* pCamera = CameraManager::instance()->GetCamera(camName);
    if( pCamera )
    {
        pCamera->start(camName);
        connect(pCamera, SIGNAL(NewImage(QImage)), &ib, SLOT(NewImage(QImage)), Qt::QueuedConnection );
    }


}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::StopCamera()
{
    //qDebug() << Q_FUNC_INFO;
    MvCamera* pCamera = CameraManager::instance()->GetCamera(camName);
    if( pCamera )
    {
        pCamera->stop();
        pCamera->disconnect();
    }
}

/// ===========================================================================
///
/// ===========================================================================
bool FormNewProductContent::GetTO(ProductTO &_TO)
{
    _TO.CAM_NAME = camName;
    return ib.GetTO(_TO);
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::StartTableDataBase()
{

    if(qApp->property("USE_DATABASE").toBool()==false) return;
    if(MSQLSingleton::instance()->GetFactorySQL()->IsOpen()==false) return;
    ib.SetAttributesDataBase( MSQLSingleton::instance()->GetFactorySQL()->GetAll());
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::StartKeepAlive()
{
    ib.StartKeepAlive();
}



/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::showEvent(QShowEvent *event)
{
    WindowManager::instance()->SetInfoTop(tr("Nova Inspeção (Câmera ") + QString("%1)").arg(camNum+1) );


    ui->btn_add->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_remove->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_down->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_up->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_camera_snap->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_zoom_area->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_zoom_fit->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_zoom_in->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_zoom_out->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::TimerColor()
{
    index_color += index_color_dir;

    if( index_color > 127 )
    {
        index_color     = 127;
        index_color_dir = -13;
    }

    if( index_color < 96 )
    {
        index_color     = 96;
        index_color_dir = 13;
    }

    QString s = QString("QSplitter::handle {\nbackground-color: rgb(255, %1, %1);\n    image: url(\":/images/splitter_icon.png\");\n}").arg(index_color+128);
    ui->splitter->setStyleSheet(s);
}

/// ===========================================================================
///
/// ===========================================================================

void FormNewProductContent::on_btn_add_clicked()
{
    DlgInspType dlg;
    if( dlg.exec() == QDialog::Rejected ) return;

    MVListWidget* mv = new MVListWidget(dlg.GetName(), dlg.GetType(), ui->lst_tools);
    MvAbstractTool* pTool = ib.InsertTool( dlg.GetType(), dlg.GetName());
    pTool->SetExecutionOrder( ui->lst_tools->count() );
    mv->SetTool( pTool );

    QListWidgetItem* p_item = new QListWidgetItem();
    p_item->setSizeHint( QSize(ui->lst_tools->viewport()->width(), 100) );
    ui->lst_tools->insertItem(ui->lst_tools->count(), p_item);
    ui->lst_tools->setItemWidget(p_item, mv);
}

/// ===========================================================================
///
/// ===========================================================================

void FormNewProductContent::on_btn_remove_clicked()
{
    if (ui->lst_tools->currentRow() < 0) return;

    QListWidgetItem* pItem     = ui->lst_tools->item(ui->lst_tools->currentRow());
    MVListWidget*    pWidget   = static_cast<MVListWidget*>(ui->lst_tools->itemWidget( pItem ));
    MvAbstractTool*  pTool     = pWidget->GetTool();

    ib.RemoveTool( pTool );
    ui->lst_tools->removeItemWidget( pItem );
    delete pItem;

    while( ui->scrollAreaWidgetContents->layout()->count() )
    {
        QLayoutItem* l = ui->scrollAreaWidgetContents->layout()->itemAt(0);
        QWidget* p = l->widget();
        ui->scrollAreaWidgetContents->layout()->removeWidget(p);
        p->close();
    }
    on_lst_tools_currentRowChanged( ui->lst_tools->currentRow() );
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::on_btn_up_clicked()
{
    int current = ui->lst_tools->currentRow();
    if(current < 1) return;

    while( ui->scrollAreaWidgetContents->layout()->count() )
    {
        QLayoutItem* l = ui->scrollAreaWidgetContents->layout()->itemAt(0);
        QWidget* p = l->widget();
        ui->scrollAreaWidgetContents->layout()->removeWidget(p);
        p->close();
    }

    ui->scrollArea->setUpdatesEnabled( false );

     QListWidgetItem* pItem     = ui->lst_tools->item(current);
     MVListWidget*    pWidget   = static_cast<MVListWidget*>(ui->lst_tools->itemWidget( pItem ));
     MvAbstractTool*  pTool     = pWidget->GetTool();

     QString name = pWidget->GetName();
     MV_TYPE type = pWidget->GetType();

     delete ui->lst_tools->takeItem(current);

     MVListWidget* mv = new MVListWidget(name, type, ui->lst_tools);
     mv->SetTool( pTool );

     QListWidgetItem* p_item = new QListWidgetItem();
     p_item->setSizeHint( QSize(ui->lst_tools->viewport()->width(), 100) );
     ui->lst_tools->insertItem(current-1, p_item);
     ui->lst_tools->setItemWidget(p_item, mv);

     ui->lst_tools->setCurrentRow( current - 1 );

     ib.ExecPosUP(pTool);

     ui->scrollArea->setUpdatesEnabled( true );
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::on_btn_down_clicked()
{
    int current = ui->lst_tools->currentRow();
    if(current >= ui->lst_tools->count()-1) return;

    while( ui->scrollAreaWidgetContents->layout()->count() )
    {
        QLayoutItem* l = ui->scrollAreaWidgetContents->layout()->itemAt(0);
        QWidget* p = l->widget();
        ui->scrollAreaWidgetContents->layout()->removeWidget(p);
        p->close();
    }

    ui->scrollArea->setUpdatesEnabled( false );

     QListWidgetItem* pItem     = ui->lst_tools->item(current);
     MVListWidget*    pWidget   = static_cast<MVListWidget*>(ui->lst_tools->itemWidget( pItem ));
     MvAbstractTool*  pTool     = pWidget->GetTool();

     QString name = pWidget->GetName();
     MV_TYPE type = pWidget->GetType();

     delete ui->lst_tools->takeItem(current);

     MVListWidget* mv = new MVListWidget(name, type, ui->lst_tools);
     mv->SetTool( pTool );

     QListWidgetItem* p_item = new QListWidgetItem();
     p_item->setSizeHint( QSize(ui->lst_tools->viewport()->width(), 100) );
     ui->lst_tools->insertItem(current+1, p_item);
     ui->lst_tools->setItemWidget(p_item, mv);

     ui->lst_tools->setCurrentRow( current + 1 );

     ib.ExecPosDN(pTool);

     ui->scrollArea->setUpdatesEnabled( true );

}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::on_lst_tools_currentRowChanged(int currentRow)
{
    if (currentRow < 0) return;

    ui->scrollArea->setUpdatesEnabled( false );

    ib.ClearSelection();

    QListWidgetItem* pItem     = ui->lst_tools->item(currentRow);
    MVListWidget*    pWidget   = static_cast<MVListWidget*>(ui->lst_tools->itemWidget( pItem ));
    MvAbstractTool*  pTool     = pWidget->GetTool();

    pTool->SetLayout(ui->scrollAreaWidgetContents->layout());
    pTool->ShowDialog();

    qApp->processEvents();

    ui->scrollArea->setUpdatesEnabled( true );
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::on_btn_camera_snap_clicked()
{
#ifndef _USE_DISK_
    SerialControl::instance()->Snap();
#else
    static QImage test_image;

    GetConfig(base_path, "DISK/BASE_PATH", QString("") );
    GetConfig(file_ext , "DISK/FILE_EXT" , QString("") );

    int rot = 0;
    GetConfig(rot , "DISK/ROTATE_ANG", 0 );

    QString fmt = "RGB";
    GetConfig(fmt , "DISK/DEST_FORMAT" , QString("RGB") );

    if( file_list.isEmpty() )
    {
        if( base_path.at(base_path.size()-1) == '/' ) base_path.remove(base_path.size()-1, 1);
        file_list = GetFileList(base_path, file_ext);
        current_pos = 0;

    }
    else
    {

        QString fname(base_path + "/" + file_list.at(current_pos) + "." + file_ext);

        QImage source(fname);
        QImage currentImage = source.convertToFormat(QImage::Format_RGB888).rgbSwapped();
        cv::Mat A = cv::Mat( cv::Size( currentImage.width(), currentImage.height()) , CV_8UC3);

        for(int i = 0; i < source.height(); i++)
        {
            memcpy(A.row(i).data, &currentImage.bits()[ currentImage.bytesPerLine() * i ], currentImage.bytesPerLine());
        }
        uchar depth = A.type() & CV_MAT_DEPTH_MASK;


        uchar chans = 1 + (A.type() >> CV_CN_SHIFT);
        if( fmt != "RGB" )
        {
            if( depth == CV_8U && chans == 3)   cv::cvtColor(A, A, cv::COLOR_RGB2GRAY);
            if( depth == CV_8U && chans == 4)   cv::cvtColor(A, A, cv::COLOR_RGBA2GRAY);
        }


        if( rot == 90 )
        {
            cv::transpose(A, A);
            cv::flip(A, A,1);
        }


        if( fmt != "RGB" )
            test_image = QImage(A.cols, A.rows, QImage::Format_Grayscale8 );
        else
            test_image = QImage(A.cols, A.rows, QImage::Format_RGB888 );

        for( int i = 0; i < A.rows; i++)
        {
            memcpy( &test_image.bits()[ test_image.bytesPerLine() * i ], A.row(i).data, A.cols * A.channels() );
        }


        ib.NewImage(test_image);

        current_pos++;
        if( current_pos == file_list.size() ) current_pos = 0;
    }
#endif
}

/// ===========================================================================
///
/// ===========================================================================
void FormNewProductContent::on_splitter_splitterMoved(int pos, int index)
{
    int a, b;
    ui->splitter->getRange(index, &a, &b);
    if( pos == a || pos == b )
    {
        if( tmColor.isActive() == false ) tmColor.start();
    }
    else
    {
        ui->splitter->setStyleSheet("QSplitter::handle {\nbackground-color: transparent;\n    image: url(\":/images/splitter_icon.png\");\n}");
        if( tmColor.isActive() == true ) tmColor.stop();
    }
}

/// ===========================================================================
///
/// ==========================================================================
void FormNewProductContent::on_btn_SincronizeDataBase_clicked()
{
    DlgDataBase  dlg;

    dlg.exec();

    if(dlg.result()== QDialog::Rejected) return ;

    MSQLSingleton::instance()->GetFactorySQL()->setAttribute("lote");

    QMap<QString,QString> table;

    if(MSQLSingleton::instance()->GetFactorySQL()->IsOpen()==false) return;
    if(qApp->property("USE_DATABASE").toBool())
        table= MSQLSingleton::instance()->GetFactorySQL()->GetByID(dlg.GetLote());

    DlgInfo dlgInfo;

    if(table.isEmpty())  dlgInfo.SetMessage(DlgInfo::IT_WARNING, tr("Falha na alteração de dados"));

    dlgInfo.SetMessage(DlgInfo::IT_WARNING,tr( "Dados alterados com sucesso"));

    dlgInfo.SetVisible(false);

    dlg.show();

    ib.SetSincronizeAttributesDataBase(table);

    dlg.close();

}
