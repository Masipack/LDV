#include "ibprocess.h"

#include <QGraphicsView>
#include <QtCore>
#include "util/systemsettings.h"

#include "mv/tools/mvfiducial.h"
#include "mv/tools/mvbarcode.h"
#include "mv/tools/mvdatamatrix.h"
#include "mv/tools/mvocr.h"

#include "TO/fiducialto.h"
#include "TO/ocrto.h"
#include "TO/barcodeto.h"
#include "TO/datamatrixto.h"

#include "util/serialcontrol.h"

#include "opencv2/highgui.hpp"

using namespace cv;

/// ===========================================================================
///
/// ===========================================================================
IBProcess::IBProcess(QObject *parent) : InspectionBuffer(parent)
{
    this->SetRunMode(Processing);

    connect(&timerKeepAlive, SIGNAL(timeout()), this, SLOT(SendKeepAlive()) );

    int idealThreadCount = QThread::idealThreadCount();
    if( idealThreadCount == -1 ) idealThreadCount = 1;

    RESIZE_ONE_DIRECTION = false;

    for(int i = 0; i < idealThreadCount -1; i++ )
    {
        QThread* p = new QThread();
         threads.push_back( p );
         p->start();
    }

    proc_id = 0;
}

/// ===========================================================================
///
/// ===========================================================================
IBProcess::~IBProcess()
{
    foreach (QThread* t, threads)
    {
        if( NULL == t ) continue;

        t->quit();
        t->wait(100);

        delete t;
        t = 0;
    }
}


/// ===========================================================================
///
/// ===========================================================================
void IBProcess::NewImage(const QImage &source)
{
    ok_response_count = 0;
    response_count    = 0;

    InspectionBuffer::NewImage(source);

    SerialControl::instance()->SetOutput(0);

    this->update();

    QPointF dist(0,0);
    proc_id++;
    for( MvAbstractTool* p : tools )
    {
        if( p->GetType() == MV_FIDUCIAL )
        {
            if( p->Exec(proc_id) == false )
            {
                emit(InspectionResult(false));
                ImageLog();
                return;
            }
            dist = qobject_cast<MvFiducial*>(p)->GetDistance();
        }
        else
        {
            p->MoveTool( p->GetConfigPos() + dist);
            if( p->Exec(proc_id) == false )
            {
                if( p->GetType() != MV_OCR ) return;
            }
        }
    }
    qApp->processEvents();

//    QString fname = "./data/image/";
//    fname += QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
//    fname += ".jpg";
//    imwrite( fname.toLatin1().data(), mat);

}

/// ===========================================================================
///
/// ===========================================================================
void IBProcess::NewResult(bool approved, const QString &value, quint32 proc_id)
{
    MvTool* p = (MvTool*)sender();

    response_count++;
    if( approved && this->proc_id == proc_id ) ok_response_count++;

    if( response_count == tools.size() )
    {
        if( ok_response_count == tools.size() )
        {
            SerialControl::instance()->ToggleBitON(number_output);
            QTimer::singleShot(IO_REJECT_TIME, this, SLOT(ClearIO()));
            emit(InspectionResult(true));
        }
        else
        {
            emit(InspectionResult(false));
            ImageLog();
        }

    }
}

/// ===========================================================================
///
/// ===========================================================================
void IBProcess::ClearIO()
{
    SerialControl::instance()->ToggleBitOFF(number_output);
}


/// ===========================================================================
///
/// ===========================================================================
void IBProcess::Reset()
{
    mat = cv::Mat();
    currentImage = QImage();

    for( MvAbstractTool* p : tools)
    {
        RemoveTool(p);
    }

    proc_id = 0;
    update();
}

/// ===========================================================================
///
/// ===========================================================================
MvAbstractTool *IBProcess::InsertTool(MV_TYPE type, const QString &name)
{
    if( type == MV_TYPE_UNDEFINED ) return nullptr;

    QRectF view_rect;
    if( this->views().isEmpty() )
        view_rect = QRectF(0,0,2448,2048);
    else
        view_rect = this->views().at(0)->mapToScene( this->views().at(0)->viewport()->geometry() ).boundingRect();

    QRectF temp_rect = QRectF(0,0, view_rect.width() / 10, view_rect.height() / 10);
    temp_rect.moveCenter( view_rect.center() );

    MvAbstractTool* pItemTool = nullptr;

    if( type == MV_FIDUCIAL )
    {
        pItemTool = new MvFiducial( temp_rect, nullptr );
        pItemTool->SetExecOnMove( true );
        pItemTool->SetResizeOnCenter( true );
        pItemTool->SetLockAngle(true);
    }

    if( type == MV_OCR )
    {
        pItemTool = new MvOCR(temp_rect, nullptr );
        pItemTool->SetExecOnMove( true );
        pItemTool->SetResizeOnCenter( false );
        pItemTool->SetLockAngle( true );
    }

    if( type == MV_DATAMATRIX )
    {
        pItemTool = new MvDataMatrix(temp_rect, nullptr );
        pItemTool->SetExecOnMove( true );
        pItemTool->SetResizeOnCenter( false );
        pItemTool->SetLockAngle( true );
    }

    if( type == MV_BARCODE )
    {
        pItemTool = new MvBarcode( temp_rect, nullptr );
        pItemTool->SetExecOnMove( true );
        pItemTool->SetResizeOnCenter( false );
        pItemTool->SetLockAngle( true );
     }

    pItemTool->SetLock( true );
    pItemTool->SetToolName( name );

    pItemTool->SetType(type);

    this->addItem( pItemTool );

    tools.append( pItemTool );


    return pItemTool;

}

/// ===========================================================================
///
/// ===========================================================================
void IBProcess::RemoveTool(MvAbstractTool *pItem)
{
    this->removeItem( pItem );

    if( tools.contains(pItem) ) tools.removeOne( pItem );
    delete pItem;

    int n = 0;
    for(MvAbstractTool* p : tools)
    {
        p->SetExecutionOrder(n++);
    }
    qSort(tools.begin(), tools.end(), ExecOrder);
}

/// ===========================================================================
///
/// ===========================================================================
void IBProcess::ImageLog()
{

    img_inspection_result = QImage(currentImage.size(), QImage::Format_ARGB32);
    img_inspection_result.convertToFormat( QImage::Format_ARGB32 );
    img_inspection_result.fill(Qt::transparent);

    QPainter painter(&img_inspection_result);

    InspectionBuffer::render(&painter,currentImage.rect(), currentImage.rect(),
                              Qt::KeepAspectRatio  );
    ImageQueue::instance()->NewImage(img_inspection_result, "Reprovado");

}

/// ===========================================================================
///
/// ===========================================================================
void IBProcess::ClearSelection()
{
    for( int i = 0; i < tools.size(); i++)
    {
        tools[i]->SetLock( true );
        tools[i]->update();
    }
}

/// ===========================================================================
///
/// ===========================================================================
void IBProcess::SendKeepAlive()
{
    SerialControl::instance()->KeepAlive();
}

/// ===========================================================================
///
/// ===========================================================================
bool IBProcess::SetTO(const ProductTO &_TO)
{

//    qDebug() << _TO;

    Reset();

    SerialControl::instance()->SetOutput(0);
    GetConfig(IO_REJECT_TIME        , "SYSTEM/IO_REJECT_TIME"       , 50);
    GetConfig(RESIZE_ONE_DIRECTION  , "SYSTEM/RESIZE_ONE_DIRECTION" , false);

    for(int i = 0; i < _TO.LIST_FIDUCIAL.size(); i++)
    {
        QRectF rc = _TO.LIST_FIDUCIAL[i].BASE.ROI;
        rc.moveCenter( _TO.LIST_FIDUCIAL[i].BASE.CONFIG_POS );

        MvFiducial* pItemTool = new MvFiducial( rc, nullptr );
        pItemTool->SetExecutionOrder( _TO.LIST_FIDUCIAL[i].BASE.EXECUTION_ORDER  );
        pItemTool->SetExecOnMove( false );
        pItemTool->SetResizeOnCenter( _TO.LIST_FIDUCIAL[i].BASE.FIXED_ANGLE );
        pItemTool->SetLockAngle(_TO.LIST_FIDUCIAL[i].BASE.FIXED_ANGLE);
        pItemTool->SetLock(true);
        pItemTool->SetConfigPos(_TO.LIST_FIDUCIAL[i].BASE.CONFIG_POS);
        pItemTool->SetImageTemplate(_TO.LIST_FIDUCIAL[i].IMG_TEMPLATE, _TO.LIST_FIDUCIAL[i].IMG_TEMPLATE_SZ);
        if(_TO.LIST_FIDUCIAL[i].BASE.FIXED_ANGLE == false)  pItemTool->setRotation( _TO.LIST_FIDUCIAL[i].BASE.ANGLE );

        if(_TO.CAM_NAME=="CAMERA_1" && RESIZE_ONE_DIRECTION ) pItemTool->SetResizeOneDirect(false);

        pItemTool->SetToolName( _TO.LIST_FIDUCIAL[i].BASE.NAME );
        this->addItem( pItemTool );

        tools.push_back( pItemTool );

        //qDebug() << "F:" << _TO.LIST_FIDUCIAL[i].BASE.NAME << rc;

        connect(pItemTool, SIGNAL(NewResult(bool,QString,quint32)), this, SLOT(NewResult(bool,QString,quint32)), Qt::QueuedConnection);
    }

    for(int i = 0; i < _TO.LIST_OCR.size(); i++)
    {
        QRectF rc = _TO.LIST_OCR[i].BASE.ROI;
        rc.moveCenter( _TO.LIST_OCR[i].BASE.CONFIG_POS );

        MvOCR* pItemTool = new MvOCR( rc, nullptr );
        pItemTool->SetExecutionOrder( _TO.LIST_OCR[i].BASE.EXECUTION_ORDER  );
        pItemTool->SetExecOnMove( false );
        pItemTool->SetResizeOnCenter( _TO.LIST_OCR[i].BASE.FIXED_ANGLE );
        pItemTool->SetLockAngle(_TO.LIST_OCR[i].BASE.FIXED_ANGLE);
        pItemTool->SetLock(true);
        pItemTool->SetConfigPos(_TO.LIST_OCR[i].BASE.CONFIG_POS);
        pItemTool->SetExpectedText(_TO.LIST_OCR[i].BASE.EXPECTED_TEXT);
        pItemTool->SetWhiteFilterSize(_TO.LIST_OCR[i].WHITE_FILTER);
        pItemTool->SetBlackAndWhite(_TO.LIST_OCR[i].BLACK_WHITE);
        pItemTool->SetAttributeDataBase(_TO.LIST_OCR[i].TABLE_ATTRIBUTE);
        pItemTool->SetTableDataBase(table);

        if(_TO.LIST_OCR[i].BASE.FIXED_ANGLE == false)
        {
              pItemTool->setTransformOriginPoint(rc.width()/2,rc.height()/2);
              pItemTool->setRotation( _TO.LIST_OCR[i].BASE.ANGLE );
              pItemTool->setPos(rc.bottomLeft());

         }


        pItemTool->SetToolName( _TO.LIST_OCR[i].BASE.NAME );
        pItemTool->GetPO()->moveToThread( threads[ tools.size() % threads.size() ]  );


        this->addItem( pItemTool );
        tools.push_back( pItemTool );

        //qDebug() << "O:" << _TO.LIST_FIDUCIAL[i].BASE.NAME << rc << _TO.LIST_OCR[i];
        connect(pItemTool, SIGNAL(NewResult(bool,QString,quint32)), this, SLOT(NewResult(bool,QString,quint32)), Qt::QueuedConnection);
    }

    for(int i = 0; i < _TO.LIST_BARCODE.size(); i++)
    {
        QRectF rc = _TO.LIST_BARCODE[i].BASE.ROI;
        rc.moveCenter( _TO.LIST_BARCODE[i].BASE.CONFIG_POS );

        MvBarcode* pItemTool = new MvBarcode( rc, nullptr );
        pItemTool->SetExecutionOrder( _TO.LIST_BARCODE[i].BASE.EXECUTION_ORDER  );
        pItemTool->SetExecOnMove( false );
        pItemTool->SetResizeOnCenter( _TO.LIST_BARCODE[i].BASE.FIXED_ANGLE );
        pItemTool->SetLockAngle(_TO.LIST_BARCODE[i].BASE.FIXED_ANGLE);
        pItemTool->SetLock(true);
        pItemTool->SetConfigPos(_TO.LIST_BARCODE[i].BASE.CONFIG_POS);
        pItemTool->SetExpectedText(_TO.LIST_BARCODE[i].BASE.EXPECTED_TEXT);

        if(_TO.LIST_BARCODE[i].BASE.FIXED_ANGLE == false)  pItemTool->setRotation( _TO.LIST_BARCODE[i].BASE.ANGLE );

        pItemTool->SetToolName( _TO.LIST_BARCODE[i].BASE.NAME );
        this->addItem( pItemTool );

        tools.push_back( pItemTool );

        //qDebug() << "B:" << _TO.LIST_BARCODE[i].BASE.NAME << rc << _TO.LIST_BARCODE[i];
        connect(pItemTool, SIGNAL(NewResult(bool,QString,quint32)), this, SLOT(NewResult(bool,QString,quint32)), Qt::QueuedConnection);
    }


    for(int i = 0; i < _TO.LIST_DATAMATRIX.size(); i++)
    {
        QRectF rc = _TO.LIST_DATAMATRIX[i].BASE.ROI;
        rc.moveCenter( _TO.LIST_DATAMATRIX[i].BASE.CONFIG_POS );

        MvDataMatrix* pItemTool = new MvDataMatrix( rc, nullptr );
        pItemTool->SetExecutionOrder( _TO.LIST_DATAMATRIX[i].BASE.EXECUTION_ORDER  );
        pItemTool->SetExecOnMove( false );
        pItemTool->SetResizeOnCenter( _TO.LIST_DATAMATRIX[i].BASE.FIXED_ANGLE );
        pItemTool->SetLockAngle(_TO.LIST_DATAMATRIX[i].BASE.FIXED_ANGLE);
        pItemTool->SetLock(true);
        pItemTool->SetConfigPos(_TO.LIST_DATAMATRIX[i].BASE.CONFIG_POS);
        pItemTool->SetExpectedText(_TO.LIST_DATAMATRIX[i].BASE.EXPECTED_TEXT);

        if(_TO.LIST_DATAMATRIX[i].BASE.FIXED_ANGLE == false)  pItemTool->setRotation( _TO.LIST_DATAMATRIX[i].BASE.ANGLE );

        pItemTool->SetToolName( _TO.LIST_DATAMATRIX[i].BASE.NAME );
        this->addItem( pItemTool );

        tools.push_back( pItemTool );

        //qDebug() << "D:" << _TO.LIST_DATAMATRIX[i].BASE.NAME << rc << _TO.LIST_DATAMATRIX[i];
        connect(pItemTool, SIGNAL(NewResult(bool,QString,quint32)), this, SLOT(NewResult(bool,QString,quint32)), Qt::QueuedConnection);
    }

    qSort(tools.begin(), tools.end(), ExecOrder);

    timerKeepAlive.start(500);

    return true;
}

///// ===========================================================================
/////
///// ===========================================================================
bool IBProcess::GetTO(ProductTO &_TO)
{

    for(int i = 0; i < tools.size(); i++  )
    {
        MvAbstractTool* p = tools[i];
        qDebug() << "TOOL " << i << "---------------------------" << p->GetType();

        if( p->GetType() == MV_FIDUCIAL )
        {
            FiducialTO to;
            MvFiducial* pTool = (MvFiducial*)p;

            if( pTool->GetImageTemplate(to.IMG_TEMPLATE, to.IMG_TEMPLATE_SZ) == false ) return false;

            to.BASE.EXECUTION_ORDER      = i;
            to.BASE.ANGLE                = pTool->rotation();
            to.BASE.CONFIG_POS           = pTool->GetConfigPos();
            to.BASE.EXPECTED_TEXT        = pTool->GetExpectedText();
            to.BASE.FIXED_ANGLE          = pTool->GetLockAngle();
            to.MIN_CORRELATION           = pTool->GetMinSimilarity();
            to.BASE.NAME                 = pTool->GetToolName();
            to.BASE.ROI                  = pTool->boundingRect();
            to.BASE.VISIBLE              = pTool->isVisible();
            to.BASE.TYPE                 = (quint32)pTool->GetType();
            _TO.LIST_FIDUCIAL.push_back( to );

           // Debug(to);
        }

        if( p->GetType() == MV_OCR )
        {
            OcrTO to;
            MvOCR* pTool = (MvOCR*)p;

            to.BASE.EXECUTION_ORDER         = i;
            to.BASE.ANGLE                   = pTool->rotation();
            to.BASE.CONFIG_POS              = pTool->GetConfigPos();
            to.BASE.EXPECTED_TEXT           = pTool->GetExpectedText();
            to.BASE.FIXED_ANGLE             = pTool->GetLockAngle();
            to.BASE.NAME                    = pTool->GetToolName();
            to.BASE.ROI                     = pTool->boundingRect();
            to.WHITE_FILTER                 = pTool->GetWhiteFilterSize();
            to.BASE.VISIBLE                 = pTool->isVisible();
            to.BASE.TYPE                    = (quint32)pTool->GetType();
            to.TABLE_ATTRIBUTE              = pTool->GetAttributeDataBase();
            _TO.LIST_OCR.push_back( to );
           // Debug(to);
        }

        if( p->GetType() == MV_BARCODE )
        {
            BarcodeTO to;
            MvBarcode* pTool = (MvBarcode*)p;
            to.BASE.EXECUTION_ORDER      = i;
            to.BASE.ANGLE                = pTool->rotation();
            to.BASE.CONFIG_POS           = pTool->GetConfigPos();
            to.BASE.EXPECTED_TEXT        = pTool->GetExpectedText();
            to.BASE.FIXED_ANGLE          = pTool->GetLockAngle();
            to.BASE.NAME                 = pTool->GetToolName();
            to.BASE.ROI                  = pTool->boundingRect();
            to.BASE.VISIBLE              = pTool->isVisible();
            to.BASE.TYPE                 = (quint32)pTool->GetType();
            _TO.LIST_BARCODE.push_back( to );
            //Debug(to);
        }

        if( p->GetType() == MV_DATAMATRIX )
        {
            DatamatrixTO to;
            MvDataMatrix* pTool = (MvDataMatrix*)p;
            to.BASE.EXECUTION_ORDER      = i;
            to.BASE.ANGLE                = pTool->rotation();
            to.BASE.CONFIG_POS           = pTool->GetConfigPos();
            to.BASE.EXPECTED_TEXT        = pTool->GetExpectedText();
            to.BASE.FIXED_ANGLE          = pTool->GetLockAngle();
            to.BASE.NAME                 = pTool->GetToolName();
            to.BASE.ROI                  = pTool->boundingRect();
            to.BASE.VISIBLE              = pTool->isVisible();
            to.BASE.TYPE                 = (quint32)pTool->GetType();
            _TO.LIST_DATAMATRIX.push_back( to );
            //Debug(to);
        }

    }

    return true;
}

///// ===========================================================================
/////
///// ===========================================================================
void IBProcess::SetAttributesDataBase(QMap<QString, QString> _table)
{
    this->table = _table;
}

///// ===========================================================================
/////
///// ===========================================================================
void IBProcess::SetSincronizeAttributesDataBase(QMap<QString, QString> _table)
{

    foreach (MvAbstractTool* tool, tools)
    {
        if(_table.contains(tool->GetAttributeDataBase()))
        {
            if(tool->GetType() == MV_OCR){
                MvOCR* pTool = (MvOCR*)tool;
                QString s =_table.value(tool->GetAttributeDataBase());
                pTool->SetExpectedText(s.remove(" "));
            }

            if(tool->GetType() == MV_DATAMATRIX){
                MvDataMatrix* pTool = (MvDataMatrix*)tool;
                QString s =_table.value(tool->GetAttributeDataBase());
                pTool->SetExpectedText(s.remove(" "));
            }

        }

    }
}
