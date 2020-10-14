#include "mvtool.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include "mv/inspectionbuffer.h"
#include "util/systemsettings.h"
#include "global_defines.h"

using namespace cv;

/// ===========================================================================
///
/// ===========================================================================
MvTool::MvTool(const QRectF& rect, QGraphicsItem *parent) :
    QGraphicsObject(parent),
    bounding_rect(0,0,rect.width(), rect.height())
{
    this->setPos( rect.topLeft() );
    this->setRotation( 0.0 );
    b_locked                        = false;
    b_lock_angle                    = true;
    drag_type                       = DRAG_NONE;
    cfg_handle_size                 = 16.0;
    b_resize_on_center              = true;
    b_resize_one_direct             = true;
    elapsed_value                   = 0;

    QFont f("FreeMono");
    f.setPixelSize( 16 );
    f.setBold( true );
    toolName = new QGraphicsSimpleTextItem(this);
    toolName->setFont(f);
    toolName->setPen(QPen(QColor(0,0,0,180) ,1));
    toolName->setBrush( QColor(0,255,0,180) );
    toolName->setPos( QPointF(0 , -19) ) ;

    GetConfig(cfg_handle_size, "TOOLS/HANDLE_SIZE", 16.0);
}

/// ===========================================================================
///
/// ===========================================================================
MvTool::~MvTool()
{

}

/// ===========================================================================
///
/// ===========================================================================
void MvTool::setIncrementSizeX(QRectF rect)
{
    this->prepareGeometryChange();
    qreal delta = rect.width() - boundingRect().width();

    bounding_rect.adjust(0, 0, b_resize_one_direct ? delta:0, b_resize_one_direct ? delta/2:delta);

    this->moveBy(b_resize_one_direct ? delta * -0.5: 0 , b_resize_one_direct ? (delta/2)*-0.5:(delta)*-0.5);

    emit( GeometryChanged( this->mapRectToScene( this->boundingRect() )) );
    this->Exec();
}

/// ===========================================================================
///
/// ===========================================================================
void MvTool::MoveTool(QPointF p)
{
    this->setPos(p - boundingRect().center());
   // Debug(this->boundingRect().center());
    emit( GeometryChanged( this->mapRectToScene( this->boundingRect() )) );
}

/// ===========================================================================
///
/// ===========================================================================
void MvTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if( b_locked )
    {
        event->ignore();
        return;
    }

    qreal scale = 1.0;

    if( this->scene() && this->scene()->views().isEmpty() == false )
    {
        scale = this->scene()->views().at(0)->transform().m11();
    }

    if( b_lock_angle == true )
    {
        QRectF handleRect( bounding_rect.bottomRight() - QPoint(cfg_handle_size/scale, cfg_handle_size/scale), bounding_rect.bottomRight() );
        if(handleRect.contains( event->pos() ) )
        {
            drag_type = DRAG_RESIZE;
        }
        else
        {
            drag_type = DRAG_MOVE;
        }

    }
    else
    {
        qreal d = cfg_handle_size/scale;
        qreal r    = d/2.0;

        QRectF rc;
        drag_type = DRAG_CENTER_ANG;

        if( bounding_rect.width() > r * 4 && bounding_rect.height() > r * 4   )
        {
            rc = QRectF( bounding_rect.right()-d, bounding_rect.center().y()-r, d, d );

            if( rc.contains( event->pos() ) )  drag_type = DRAG_RIGHT_ANG;

            rc = QRectF( bounding_rect.center().x()-r, bounding_rect.bottom()-d, d, d );

            if( rc.contains( event->pos() ) )  drag_type = DRAG_BOTTOM_ANG;
        }
    }
}

/// ===========================================================================
///
/// ===========================================================================
void MvTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    qreal scale = 1.0;
    if( this->scene() && this->scene()->views().isEmpty() == false )
    {
        scale = this->scene()->views().at(0)->transform().m11();
    }

    if(  drag_type == DRAG_RESIZE )
    {
        this->prepareGeometryChange();
        QPointF delta = event->scenePos() - event->lastScenePos();

        QRectF handleRect( bounding_rect.bottomRight() - QPoint(cfg_handle_size/scale, cfg_handle_size/scale), bounding_rect.bottomRight() );

        if( this->boundingRect().width()  <= handleRect.width()  && delta.x() < 0 ) delta.setX(0);
        if( this->boundingRect().height() <= handleRect.height() && delta.y() < 0 ) delta.setY(0);

        if( b_resize_on_center )
            bounding_rect.adjust(0, 0, delta.x() * 2.0, delta.y() * 2.0);
        else
            bounding_rect.adjust(0, 0, delta.x(), delta.y());

        if( bounding_rect.width()  < handleRect.width() )  bounding_rect.setWidth(handleRect.width());
        if( bounding_rect.height() < handleRect.height() ) bounding_rect.setHeight(handleRect.height());

        if( b_resize_on_center ) this->moveBy(delta.x() * -1.0 , delta.y() * -1.0 );

        emit( GeometryChanged( this->mapRectToScene( this->boundingRect() )) );
        if( b_exec_on_move ) this->Exec();
        return;
    }

    if(  drag_type == DRAG_MOVE || drag_type == DRAG_CENTER_ANG )
    {
        QPointF delta = event->scenePos() - event->lastScenePos();
        this->setPos(pos() + delta);
        emit( GeometryChanged( this->mapRectToScene( this->boundingRect() )) );
        if( b_exec_on_move ) this->Exec();
        return;
    }

    if(  drag_type == DRAG_RIGHT_ANG  )
    {
        QPointF c = bounding_rect.center();

        QLineF L1(c, event->pos());
        QLineF L2(c, event->lastPos());

        qreal angle = fmod((L1.angle() - this->rotation()) * -1.0, 360.0);

        qreal dx = L2.dx() - L1.dx();

        if( (boundingRect().width() + (-dx)) > ((cfg_handle_size/scale) * 2.2) )
        {
            this->moveBy(dx, 0);
            bounding_rect.adjust(0.0, 0.0, -dx * 2.0, 0.0);
        }
        this->setRotation( angle );


        emit( GeometryChanged( this->mapRectToScene( this->boundingRect() )) );
        if( b_exec_on_move ) this->Exec();

        setTransformOriginPoint( boundingRect().center() );
        return;
    }

    if(   drag_type == DRAG_BOTTOM_ANG )
    {
        QPointF c = bounding_rect.center();

        QLineF L1(c, event->pos());
        QLineF L2(c, event->lastPos());

        qreal angle = fmod((L1.angle() - this->rotation()) * -1.0, 360.0) - 90.0;

        qreal dy = L2.dy() - L1.dy();

        if( (boundingRect().height() + (-dy)) > ((cfg_handle_size/scale) * 2.2) )
        {
            this->moveBy(0, dy);
            bounding_rect.adjust(0.0, 0.0, 0.0, -dy * 2.0);
        }

        this->setRotation( angle );

        emit( GeometryChanged( this->mapRectToScene( this->boundingRect() )) );
        if( b_exec_on_move ) this->Exec();

        setTransformOriginPoint( boundingRect().center() );
        return;
    }

    QGraphicsObject::mouseMoveEvent(event);
}

/// ===========================================================================
///
/// ===========================================================================
void MvTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    drag_type = DRAG_NONE;
    update();
}

/// ===========================================================================
///
/// ===========================================================================
void MvTool::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    qreal scale = 1.0;

    if( this->scene() && this->scene()->views().isEmpty() == false )
    {
        scale = this->scene()->views().at(0)->transform().m11();
    }

    QRectF handleRect( bounding_rect.bottomRight() - QPoint(cfg_handle_size/scale, cfg_handle_size/scale), bounding_rect.bottomRight() );

    QPen pn(Qt::black, 3);
    pn.setCosmetic( true );
    painter->setPen( pn );
    PaintTool(painter, scale, handleRect);

    if( b_locked )
    {
        QPen pn_inside(Qt::gray, 1);
        pn_inside.setCosmetic( true );
        painter->setPen( pn_inside );
        PaintTool(painter, scale, handleRect);
    }
    else
    {
        QPen pn_inside(Qt::green, 1);
        pn_inside.setCosmetic( true );
        painter->setPen( pn_inside );
        PaintTool(painter, scale, handleRect);
    }

}

/// ===========================================================================
///
/// ===========================================================================
void MvTool::PaintTool(QPainter *painter, qreal scale, QRectF& handleRect)
{
    painter->drawRect(  bounding_rect );

    if( b_lock_angle == true )
    {
        if(  b_resize_on_center && (drag_type == DRAG_RESIZE || drag_type == DRAG_MOVE) )
        {
            qreal r = 3.0/scale;
            painter->drawEllipse( bounding_rect.center(), r, r );
        }

        if( bounding_rect.width() > handleRect.width() && bounding_rect.height() > handleRect.height()   )
        {
            if( b_locked == false ) painter->drawRect( handleRect );
        }
        else
        {
            if( handleRect.height() >= bounding_rect.height() ) handleRect.setHeight( bounding_rect.height() );
            if( handleRect.width()  >= bounding_rect.width()  ) handleRect.setWidth(  bounding_rect.width() );
            handleRect.moveBottomRight( bounding_rect.bottomRight()  );
            painter->drawRect( handleRect );
        }
    }
    else
    {
        qreal d = cfg_handle_size/scale;
        qreal r = d/2.0;

        if(  drag_type != DRAG_NONE )
        {
            painter->drawEllipse( bounding_rect.center(), r/2, r/2 );
        }

        if( b_locked == false && bounding_rect.width() > r * 4 && bounding_rect.height() > r * 4   )
        {
            painter->drawEllipse( QPointF(bounding_rect.right()- r  , bounding_rect.center().y()), r, r );
            painter->drawEllipse( QPointF(bounding_rect.center().x(), bounding_rect.bottom()-r ) , r,r );
        }
    }
}

/// ===========================================================================
///
/// ===========================================================================
void MvTool::rotate2D(const cv::Mat & src, cv::Mat & dst, const double degrees)
{
    cv::Point2f center(src.cols/2.0, src.rows/2.0);

    cv::Mat rot         = cv::getRotationMatrix2D(center, degrees, 1.0);
    cv::Rect bbox       = cv::RotatedRect(center,src.size(), degrees).boundingRect();

    rot.at<double>(0,2) += bbox.width/2.0  - center.x;
    rot.at<double>(1,2) += bbox.height/2.0 - center.y;

    cv::warpAffine(src, dst, rot, bbox.size());
}


/// ===========================================================================
///
/// ===========================================================================
bool MvTool::ExtractRoi(bool bCopy)
{
    if( this->scene() == 0 ) return false;
    InspectionBuffer* pBuf = qobject_cast<InspectionBuffer*>(this->scene());

    QRectF rc = mapRectToScene( boundingRect() );

    if( b_lock_angle  )
    {
        if( pBuf->ToolImage(rc, roi, bCopy) == false )
            return false;

        if( roi.data == 0 )
            return false;

        return true;
    }

    if( b_lock_angle == false )
    {
        QRectF rs = this->mapRectToScene(bounding_rect);
        cv::Rect bounds = cv::RotatedRect( cv::Point2f(rs.center().x(), rs.center().y()), cv::Size(bounding_rect.width(), bounding_rect.height()), this->rotation()).boundingRect();


        if( pBuf->ToolImage(QRectF(bounds.tl().x,bounds.tl().y,bounds.width,bounds.height), roi, bCopy) == false )
            return false;

        if( roi.data == 0 )
            return false;

        if( roi.cols < bounds.width || roi.rows < bounds.height )
        {
            cv::Mat nee(bounds.height,bounds.width, roi.type(), cv::Scalar::all(0) );

            QRectF rrc = mapRectToScene( bounding_rect );
            int x = 0, y = 0;
            if( rrc.left() < 0 ) x = qAbs(rrc.left());
            if( rrc.top()  < 0 ) y = qAbs(rrc.top());

            roi.copyTo(nee(cv::Rect(x,y,roi.cols, roi.rows)));

            nee.copyTo( roi );
        }

        rotate2D(roi, roi, this->rotation() );

        cv::Rect rc_crop(qCeil(roi.cols - bounding_rect.width())  / 2,
                         qCeil(roi.rows - bounding_rect.height()) / 2,
                         qFloor(bounding_rect.width()),
                         qFloor(bounding_rect.height()) );

        roi = roi(rc_crop);

    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvTool::ExtractRoi(QRectF src_rect, bool bCopy)
{
    if( this->scene() == 0 ) return false;

    InspectionBuffer* pBuf = qobject_cast<InspectionBuffer*>(this->scene());

    QRectF rc = mapRectToScene( src_rect );

    if( b_lock_angle  )
    {
        if( pBuf->ToolImage(rc, roi, bCopy) == false )
            return false;

        if( roi.data == 0 )
            return false;

        return true;
    }

    if( b_lock_angle == false )
    {
        QRectF rs = this->mapRectToScene( src_rect );
        cv::Rect bounds = cv::RotatedRect( cv::Point2f(rs.center().x(), rs.center().y()), cv::Size(src_rect.width(), src_rect.height()), this->rotation()).boundingRect();


        if( pBuf->ToolImage(QRectF(bounds.tl().x,bounds.tl().y,bounds.width,bounds.height), roi, bCopy) == false )
            return false;

        if( roi.data == 0 )
            return false;

        if( roi.cols < bounds.width || roi.rows < bounds.height )
        {
            cv::Mat nee(bounds.height,bounds.width, roi.type(), cv::Scalar::all(0) );

            QRectF rrc = mapRectToScene( src_rect );
            int x = 0, y = 0;
            if( rrc.left() < 0 ) x = qAbs(rrc.left());
            if( rrc.top()  < 0 ) y = qAbs(rrc.top());

            roi.copyTo(nee(cv::Rect(x,y,roi.cols, roi.rows)));

            nee.copyTo( roi );
        }

        rotate2D(roi, roi, this->rotation() );

        cv::Rect rc_crop(qCeil(roi.cols - src_rect.width())  / 2,
                         qCeil(roi.rows - src_rect.height()) / 2,
                         qFloor(src_rect.width()),
                         qFloor(src_rect.height()) );

        if( rc_crop.width <= 8 || rc_crop.height <= 8 ) return false;

        roi = roi(rc_crop);

    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvTool::ExtractRoiGray()
{
    if( this->scene() == 0 ) return false;
    if( ExtractRoi(true) == false ) return false;

    uchar depth = roi.type() & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (roi.type() >> CV_CN_SHIFT);

    if( depth == CV_8U && chans == 1)
    {
        return true;
    }

    if( depth == CV_8U && chans == 3)
    {
        cv::cvtColor(roi, roi, cv::COLOR_RGB2GRAY);
        return true;
    }

    if( depth == CV_8U && chans == 4)
    {
        cv::cvtColor(roi, roi, cv::COLOR_RGBA2GRAY);
        return true;
    }

    return false;
}

/// ===========================================================================
///
/// ===========================================================================
qreal MvTool::GetAngle(const cv::RotatedRect& src)
{
    QVector<QPointF> pts;

    Point2f vertices[4];
    src.points(vertices);

    if(src.size.width > src.size.height)
    {
        QLineF L1(QPointF(vertices[1].x, vertices[1].y), QPointF(vertices[2].x, vertices[2].y) );
        return qFabs(L1.angle());
    }

    QLineF L1(QPointF(vertices[2].x, vertices[2].y), QPointF(vertices[3].x, vertices[3].y) );
    return qFabs(L1.angle());
}
