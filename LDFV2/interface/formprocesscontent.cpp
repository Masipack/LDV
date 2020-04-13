#include "formprocesscontent.h"
#include "ui_formprocesscontent.h"

#include "interface/windowmanager.h"
#include "interface/widgets/mvlistwidget.h"
#include "interface/formstatistics.h"
#include "util/systemsettings.h"
#include "util/fileutil.h"
#include "mv/camera/cameramanager.h"
#include "util/serialcontrol.h"
#include "util/dlginfo.h"
#include "util/dlgDataBase.h"
#include "util/fileutil.h"
#include "util/msqlsingleton.h"

/// ===========================================================================
///
/// ===========================================================================
FormProcessContent::FormProcessContent(QWidget *parent) : QWidget(parent),
    ui(new Ui::FormProcessContent)
{
    ui->setupUi(this);

    tmColor.setInterval(150);
    index_color         = 0;
    index_color_dir     = 13;

    frmStatistics = new FormStatistics();

    connect(&tmColor, SIGNAL(timeout()), this, SLOT(TimerColor()));
    connect(&ib, SIGNAL(InspectionResult(bool)), frmStatistics, SLOT(NewInsp(bool)),Qt::QueuedConnection);

    QList<int> list;
    list << 529 << 124;
    ui->splitter->setSizes(list);

    ui->imagedevice->setScene(&ib);

    ui->btn_sincrinizeDataBase->setVisible(qApp->property("USE_DATABASE").toBool());
}

/// ===========================================================================
///
/// ===========================================================================
FormProcessContent::~FormProcessContent()
{
    delete ui;
    delete frmStatistics;
}

/// ===========================================================================
///
/// ===========================================================================
void FormProcessContent::showEvent(QShowEvent *event)
{
    WindowManager::instance()->SetInfoTop(tr("Inspeção (Câmera ") + QString("%1)").arg(camNum+1)+QString(": %1").arg(configFilename) );
}

/// ===========================================================================
///
/// ===========================================================================
void FormProcessContent::TimerColor()
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
bool FormProcessContent::SetTO(const ProductTO &src)
{
    if( ib.SetTO( src ) == false ) return false;

    ProdutoTOChanges = src;


    QList<MvAbstractTool*>  tools;
    ib.GetTools(tools);

    for(int i = 0; i < tools.size(); i++)
    {
        MVListWidget* mv = new MVListWidget(tools[i]->GetToolName(), tools[i]->GetType(), ui->lst_tools);
        mv->SetTool(tools[i]);

        QListWidgetItem* p_item = new QListWidgetItem();
        p_item->setSizeHint( QSize(ui->lst_tools->viewport()->width(), 100) );
        ui->lst_tools->insertItem(ui->lst_tools->count(), p_item);
        ui->lst_tools->setItemWidget(p_item, mv);
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void FormProcessContent::SetCamera(const QString &name, int n)
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
void FormProcessContent::StopCamera()
{
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
bool FormProcessContent::GetTO(ProductTO &_TO)
{
    _TO.CAM_NAME  = ProdutoTOChanges.CAM_NAME;
    return  ib.GetTO(_TO);
}

/// ===========================================================================
///
/// ===================/// ===========================================================================
///
/// ===================================================================================================================================
void FormProcessContent::on_lst_tools_currentRowChanged(int currentRow)
{
    if (currentRow < 0)
    {
        while( ui->scrollAreaWidgetContents->layout()->count() )
        {
            QLayoutItem* l = ui->scrollAreaWidgetContents->layout()->itemAt(0);
            QWidget* p = l->widget();
            ui->scrollAreaWidgetContents->layout()->removeWidget(p);
            p->close();
        }

        ib.ClearSelection();
        return;
    }

    ui->btn_Statistics->setChecked(false);

    ui->scrollArea->setUpdatesEnabled( false );

    ib.ClearSelection();

    QListWidgetItem* pItem     = ui->lst_tools->item(currentRow);
    MVListWidget*    pWidget   = static_cast<MVListWidget*>(ui->lst_tools->itemWidget( pItem ));
    MvAbstractTool*  pTool     = pWidget->GetTool();

    pTool->SetLayout(ui->scrollAreaWidgetContents->layout());
    pTool->ShowDialog(false);

    qApp->processEvents();

    ui->scrollArea->setUpdatesEnabled( true );
}

/// ===========================================================================
///
/// ===========================================================================
void FormProcessContent::on_btn_camera_snap_clicked()
{
#ifndef _USE_DISK_
    SerialControl::instance()->Snap();
#else/// ===========================================================================
    ///
    /// ===========================================================================
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

        Debug(fname)
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
void FormProcessContent::on_splitter_splitterMoved(int pos, int index)
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
/// ===========================================================================
void FormProcessContent::on_btn_Statistics_toggled(bool checked)
{
    while( ui->scrollAreaWidgetContents->layout()->count() )
    {
        QLayoutItem* l = ui->scrollAreaWidgetContents->layout()->itemAt(0);
        QWidget* p = l->widget();
        ui->scrollAreaWidgetContents->layout()->removeWidget(p);
        p->close();
    }

    ib.ClearSelection();
    /// ===========================================================================
    ///
    /// ===========================================================================
    if(checked)
    {
       ui->scrollArea->setUpdatesEnabled( false );

       ui->lst_tools->setCurrentRow(-1);
       QLayout* pLayout = ui->scrollAreaWidgetContents->layout();

       pLayout->addWidget( frmStatistics  );
       frmStatistics->showNormal();

       qApp->processEvents();

       ui->scrollArea->setUpdatesEnabled( true );

       ProductTO ConfigTO;
       ib.GetTO(ConfigTO);

       if(ChangesTOOLS(ConfigTO))
       {
         emit(WriteConfig(configFilename));
       }
    }

}

/// ===========================================================================
///
/// ===========================================================================
void FormProcessContent::on_btn_ImageLog_clicked()
{
    WindowManager::instance()->ShowScreen( "ImageLog" );
}

/// ===========================================================================
///
/// ===========================================================================
bool FormProcessContent::ChangesTOOLS(ProductTO& other)
{
    for(int i = 0; i < ProdutoTOChanges.LIST_FIDUCIAL.size(); i++)
    {
      if(ProdutoTOChanges.LIST_FIDUCIAL[i].hasChanges(other.LIST_FIDUCIAL[i])) return true ;
    }

    for(int i = 0; i < ProdutoTOChanges.LIST_OCR.size(); i++)
    {
      if(ProdutoTOChanges.LIST_OCR[i].hasChanges(other.LIST_OCR[i])) return true;
    }

    for(int i = 0; i < ProdutoTOChanges.LIST_DATAMATRIX.size(); i++)
    {
      if(ProdutoTOChanges.LIST_DATAMATRIX[i].hasChanges(other.LIST_DATAMATRIX[i])) return true;
    }

    for(int i = 0; i < ProdutoTOChanges.LIST_BARCODE.size(); i++)
    {
      if(ProdutoTOChanges.LIST_BARCODE[i].hasChanges(other.LIST_BARCODE[i])) return true;
    }

    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void FormProcessContent::on_btn_sincrinizeDataBase_clicked()
{

    if( P11(tr("Alterando atributes na receita"), true ) == false ) return;

    DlgDataBase  dlg;

    dlg.exec();

    if(dlg.result()== QDialog::Rejected) return ;

    MSQLSingleton::instance()->GetFactorySQL()->setAttribute("lote");

    QMap<QString,QString> table;

    if(MSQLSingleton::instance()->GetFactorySQL()->IsOpen()==false) return;
    if(qApp->property("USE_DATABASE").toBool())
        table= MSQLSingleton::instance()->GetFactorySQL()->GetByID(dlg.GetLote());

    DlgInfo dlgInfo;

    if (table.isEmpty())  dlgInfo.SetMessage(DlgInfo::IT_WARNING, " Falha na alteração de dados");

    dlgInfo.SetMessage(DlgInfo::IT_WARNING, " Dados alterados com sucesso");

    dlgInfo.SetVisible(false);

    dlg.show();

    ib.SetSincronizeAttributesDataBase(table);

    dlg.close();

}
