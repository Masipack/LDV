#include "inspectionbuffer.h"
#include <QGraphicsView>

#include <QDebug>
#include <QtGui>
#include <QtWidgets>
#include <mv/mvtool.h>

#include "util/fileutil.h"
#include "mv/imagedevice.h"

/// ===========================================================================
///
/// ===========================================================================
InspectionBuffer::InspectionBuffer(QObject *parent) : QGraphicsScene(parent)
{
    this->setBackgroundBrush(Qt::darkGray);
    runMode = Processing;

}

/// ===========================================================================
///
/// ===========================================================================
void InspectionBuffer::FitInView()
{

    QRectF r;

    foreach (QGraphicsItem* i, this->items())
    {
        if( i->isVisible() == false ) continue;
        r = i->mapRectToScene( i->boundingRect() ).united( r );
    }

    if( r.isEmpty() )  r = this->sceneRect();

    foreach (QGraphicsView* p, this->views())
    {
        if( p->isVisible() )
        {
            p->fitInView( r.adjusted(-10,-10,10,10), Qt::KeepAspectRatio);
        }
    }
}

/// ===========================================================================
///
/// ===========================================================================
void InspectionBuffer::FitInView(QGraphicsView* pView)
{

    QRectF r;

    foreach (QGraphicsItem* i, this->items())
    {
        if( i->isVisible() == false ) continue;
        r = i->mapRectToScene( i->boundingRect() ).united( r );
    }

    if( r.isEmpty() )  r = this->sceneRect();

    if( this->views().size() )
    {
        pView->fitInView( r.adjusted(-10,-10,10,10), Qt::KeepAspectRatio);
    }

}

/// ===========================================================================
///
/// ===========================================================================
void InspectionBuffer::lockParent(bool v)
{
    foreach (QGraphicsItem* i, this->items())
    {
        if( i->parentItem() == 0 )
        {
            MvTool* p = (MvTool*)i;
            p->SetLock( v );
        }
    }
    this->update();
}

/// ===========================================================================
///
/// ===========================================================================
void InspectionBuffer::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setRenderHint( (QPainter::RenderHint)0x0F );
    painter->fillRect(rect, this->backgroundBrush());

    if( currentImage.isNull() ) return;
    painter->drawImage(this->sceneRect(), currentImage, currentImage.rect() );

}

/// ===========================================================================
///
/// ===========================================================================
void InspectionBuffer::drawItems(QPainter *painter,
                                 int numItems, QGraphicsItem *items[],
                                 const QStyleOptionGraphicsItem options[], QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(numItems)
    Q_UNUSED(items)
    Q_UNUSED(options)
    Q_UNUSED(widget)

    for (int i = 0; i < numItems; ++i)
    {
         painter->save();
         painter->setMatrix(items[i]->sceneMatrix(), true);
         items[i]->paint(painter, &options[i], widget);
         painter->restore();
     }

}

/// ===========================================================================
///
/// ===========================================================================
void InspectionBuffer::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(painter)
    Q_UNUSED(rect)
    painter->setRenderHint( (QPainter::RenderHint)0x0F );
    QGraphicsScene::drawForeground(painter, rect);
}

/// ===========================================================================
///
/// ===========================================================================
void InspectionBuffer::NewImage(const QImage &source)
{
    if( source.isNull() ) return;

    bool bFit = currentImage.isNull();

    switch(source.depth())
    {
        case( 32 ):
            mat = cv::Mat( cv::Size( source.width(), source.height()) , CV_8UC4, (void*)source.bits() );
            currentImage = source.convertToFormat(QImage::Format_ARGB32).rgbSwapped();
            break;
        case( 24 ):
            mat = cv::Mat( cv::Size( source.width(), source.height()) , CV_8UC3, (void*)source.bits() );
            currentImage = source.convertToFormat(QImage::Format_ARGB32).rgbSwapped();
            break;
        case( 8 ):
            mat = cv::Mat( cv::Size( source.width(), source.height()) , CV_8UC1, (void*)source.bits() );
            currentImage = source.convertToFormat(QImage::Format_ARGB32).rgbSwapped();
            break;
         default:
            return;
    }
    this->setSceneRect( currentImage.rect() );

    if( bFit && this->views().size() ) this->views().at(0)->fitInView(currentImage.rect(), Qt::KeepAspectRatio);
}


/// ===========================================================================
///
/// ===========================================================================
void InspectionBuffer::addItem(QGraphicsObject *item)
{
    QGraphicsScene::addItem( item );
}

/// ===========================================================================
///
/// ===========================================================================
bool InspectionBuffer::ToolImage(const QRectF &roi_rect, cv::Mat& mat_dest, bool deepCopy )
{
    if( currentImage.isNull() )                             return false;
    if( mat.empty() )                                       return false;
    if( this->sceneRect().intersects(roi_rect) == false )   return false;

    QRectF rect_util = this->sceneRect().normalized().intersected(roi_rect);

    if( rect_util.width()  < 1 )   return false;
    if( rect_util.height() < 1 )   return false;

    cv::Rect rr = cv::Rect( qFloor(rect_util.x()), qFloor(rect_util.y()), qCeil(rect_util.width()), qCeil(rect_util.height()));

    if( deepCopy )
    {
        cv::Mat p = mat( rr );
        p.copyTo(mat_dest);
    }
    else
    {
        mat_dest = mat( rr );
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
QImage InspectionBuffer::ToolImage( const QRectF &roi_rect )
{
    if( currentImage.isNull() ) return QImage();

    return currentImage.copy( roi_rect.toRect() );
}
