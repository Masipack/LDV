#include "imagedevice.h"
#include <QtOpenGL/QGL>

#include "mv/inspectionbuffer.h"

/// ===========================================================================
///
/// ===========================================================================
ImageDevice::ImageDevice(QWidget *parent) : QGraphicsView(parent)
{
#ifndef _WIN32
//    setViewport(new QGLWidget());
#endif

    this->setRenderHints( QPainter::Antialiasing  | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing );
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setFrameShadow(QFrame::Plain);
    this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );

    this->setCacheMode(QGraphicsView::CacheNone);

    pEmptyScene = new QGraphicsScene(this);
    this->setScene( pEmptyScene );
}

/// ===========================================================================
///
/// ===========================================================================
ImageDevice::~ImageDevice()
{
    if( pEmptyScene )
    {
        delete pEmptyScene;
        pEmptyScene = NULL;
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ImageDevice::Clear()
{
    this->setScene( pEmptyScene );
}

/// ===========================================================================
///
/// ===========================================================================
void ImageDevice::zoomFit()
{
    if( this->scene() )
    {
        fitInView(sceneRect(), Qt::KeepAspectRatio);
        emit( scaleChanged( this->transform().m11() ) );
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ImageDevice::zoomIn()
{
    scale(1.1, 1.1);
    emit( scaleChanged( this->transform().m11() ) );
}

/// ===========================================================================
///
/// ===========================================================================
void ImageDevice::zoomOut()
{
    scale(1 / 1.1, 1 / 1.1);
    emit( scaleChanged( this->transform().m11() ) );
}

/// ===========================================================================
///
/// ===========================================================================
void ImageDevice::zoomFitItems()
{
    InspectionBuffer* buf = (InspectionBuffer*)this->scene();
    if( buf == 0 ) return;
    buf->FitInView();
}

/// ===========================================================================
///
/// ===========================================================================
void ImageDevice::resetTransform()
{
    scale(1.0, 1.0);
    emit( scaleChanged( 1.0 ) );

    QGraphicsView::resetTransform();
}

/// ===========================================================================
///
/// ===========================================================================
void ImageDevice::lockParent(bool v)
{
    if( this->scene() )
    {
        QMetaObject::invokeMethod(this->scene(), "lockParent", Qt::DirectConnection, Q_ARG(bool, v) );
    }
}

/// ===========================================================================
///
/// ===========================================================================
void ImageDevice::snap()
{
    if( this->scene() )
    {
        QMetaObject::invokeMethod(this->scene(), "snap", Qt::QueuedConnection );
    }
}

