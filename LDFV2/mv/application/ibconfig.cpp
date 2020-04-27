#include "ibconfig.h"

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

using namespace cv;

/// ===========================================================================
///
/// ===========================================================================
IBConfig::IBConfig(QObject *parent) : InspectionBuffer(parent)
{
    this->SetRunMode(Interactive);

    connect(&timerKeepAlive, SIGNAL(timeout()), this, SLOT(SendKeepAlive()) );
}

/// ===========================================================================
///
/// ===========================================================================
IBConfig::~IBConfig()
{
}


/// ===========================================================================
///
/// ===========================================================================
void IBConfig::NewImage(const QImage &source)
{
    InspectionBuffer::NewImage(source);

    qSort(tools.begin(), tools.end(), ExecOrder);

    QPointF dist(0,0);
    for( MvAbstractTool* p : tools )
    {
        if( p->GetType() == MV_FIDUCIAL )
        {
            if( p->Exec(0) == false )                continue;
            dist = qobject_cast<MvFiducial*>(p)->GetDistance();
            p->MoveTool( p->GetConfigPos() + dist);
        }
        else
        {
            p->MoveTool( p->GetConfigPos() + dist);
            if( p->Exec(0) == false )                continue;
        }
    }

    for( MvAbstractTool* p : tools )
    {
        p->ReconfigPosition();
    }

    this->update();
}

/// ===========================================================================
///
/// =========================================================================
void IBConfig::SendKeepAlive()
{
    SerialControl::instance()->KeepAlive();
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfig::Reset()
{
    mat = cv::Mat();
    currentImage = QImage();
    update();
}

/// ===========================================================================
///
/// ===========================================================================
MvAbstractTool *IBConfig::InsertTool(MV_TYPE type, const QString &name)
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
        pItemTool->SetTableDataBase(table);

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
void IBConfig::RemoveTool(MvAbstractTool *pItem)
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
void IBConfig::ExecPosUP(MvAbstractTool *pItem)
{
    for(int i = 1; i < tools.size(); i++)
    {
        if( tools[i] == pItem )
        {
            tools[i-1]->SetExecutionOrder( tools[i-1]->GetExecutionOrder() + 1 );
            tools[i]->SetExecutionOrder( tools[i]->GetExecutionOrder() - 1 );
        }
    }
    qSort(tools.begin(), tools.end(), ExecOrder);
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfig::ExecPosDN(MvAbstractTool *pItem)
{
    for(int i = 0; i < tools.size()-1; i++)
    {
        if( tools[i] == pItem )
        {
            tools[i+1]->SetExecutionOrder( tools[i+1]->GetExecutionOrder() - 1 );
            tools[i]->SetExecutionOrder( tools[i]->GetExecutionOrder() + 1 );
        }
    }
    qSort(tools.begin(), tools.end(), ExecOrder);
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfig::ClearSelection()
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
bool IBConfig::GetTO(ProductTO &_TO)
{

    for(int i = 0; i < tools.size(); i++  )
    {
        MvAbstractTool* p = tools[i];
        //qDebug() << "TOOL " << i << "---------------------------" << p->GetType();

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
//            qDebug() << to;
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
            to.TABLE_ATTRIBUTE              = pTool->GetAttributeDataBase();
            to.BLACK_WHITE                  = pTool->GetBlackAndWhite();
            to.BASE.TYPE                    = (quint32)pTool->GetType();
            _TO.LIST_OCR.push_back( to );
//            qDebug() << to;
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
//            qDebug() << to;
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
//            qDebug() << to;
        }

    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfig::SetAttributesDataBase(QMap<QString, QString> _table)
{
    table = _table;
}


/// ===========================================================================
///
/// ===========================================================================
void IBConfig::SetSincronizeAttributesDataBase(QMap<QString, QString> _table)
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

/// ===========================================================================
///
/// ===========================================================================
void IBConfig::StartKeepAlive()
{
    timerKeepAlive.start(500);
}
