#ifndef MVTOOL_H
#define MVTOOL_H

//#include "global_defines.h"
#include <QGraphicsObject>
#include <QtCore>
#include <QtGui>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>


class MvTool : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MvTool(const QRectF& rect, QGraphicsItem* parent = 0);
    virtual ~MvTool();

    // Funcoes virtuais puras
    virtual bool Exec()                 = 0;
    virtual void Clear()                = 0;
    virtual bool ShowDialog(bool bEnable=true)           = 0;
    virtual void Reset()                = 0;

    virtual QRectF boundingRect() const { return bounding_rect; }

    void   setIncrementSizeX(QRectF rect);

    quint32 GetElapsedTime() { return elapsed_value; }

    void MoveTool( QPointF p );

    void SetLock( bool v )                  { b_locked = v; }
    void SetToolName(const QString& name)   { this->setObjectName(name); toolName->setText(name); }
    void SetLockAngle( bool v )             { b_lock_angle = v;         }
    void SetResizeOnCenter( bool v )        { b_resize_on_center = v;   }
    void SetExecOnMove( bool v )            { b_exec_on_move = v;       }
    void SetResizeOneDirect(bool v)         { b_resize_one_direct = v;  }

    QString GetToolName()                   { return toolName->text(); }

    bool GetLock()                          { return b_locked; }
    bool GetLockAngle()                     { return b_lock_angle; }

    static qreal GetAngle(const cv::RotatedRect& src);

signals:
    void GeometryChanged(const QRectF& rect );

public slots:


protected:

    class MvTM
    {
    public:
        MvTM(MvTool* mv)  { p = mv; p->ExecStart(); }
        ~MvTM() { p->ExecEnd(); /*qDebug() << p->GetElapsedTime();*/   }

        MvTool* p;
    };

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void ExecStart() { elapsed_timer.start(); }

    void ExecEnd()   { elapsed_value = elapsed_timer.elapsed(); }

    bool ExtractRoi(bool bCopy = false);

    bool ExtractRoi(QRectF src_rect, bool bCopy = false);

    bool ExtractRoiGray();

protected:
    cv::Mat             roi;
    bool                b_exec_on_move;
    bool                b_resize_on_center;
    bool                b_lock_angle;
    bool                b_locked;
    bool                b_resize_one_direct;

    QGraphicsSimpleTextItem*    toolName;

    qreal               cfg_handle_size;

private:
    enum DRAG_TYPE { DRAG_NONE, DRAG_MOVE, DRAG_RESIZE, DRAG_RIGHT_ANG, DRAG_BOTTOM_ANG, DRAG_CENTER_ANG };

    QRectF              bounding_rect;
    DRAG_TYPE           drag_type;
    quint32             elapsed_value;
    QElapsedTimer       elapsed_timer;



private:
    void PaintTool(QPainter *painter, qreal scale, QRectF &handleRect);

    void rotate2D(const cv::Mat & src, cv::Mat & dst, const double degrees);
};

#define TM_MEASURE QScopedPointer<MvTool::MvTM> ptm( new MvTM(this) );

#endif // MVTOOL_H
