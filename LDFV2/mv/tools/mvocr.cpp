#include "mvocr.h"

#include <QtCore>
#include <opencv2/highgui.hpp>
#include <QPainter>
#include <QGraphicsView>
#include "global_defines.h"
#include "util/fileutil.h"
#include "util/systemsettings.h"

using namespace cv;



/// ===========================================================================
///
/// ===========================================================================
MvOCR::MvOCR(const QRectF& rect, MvTool *parent) : MvAbstractTool(rect, parent)
{
    form = new  ParamsOCR(this);
    PO.Init();

    b_Busy       = false;

    white_filter    = 0;

    b_black_And_white = false;

    connect(&PO, SIGNAL(ExecResult(QImage,QString,quint32)), this, SLOT(ExecResult(QImage,QString,quint32)), Qt::QueuedConnection );

    if(qApp->property("USE_DATABASE").toBool())
    {
      connect(form, SIGNAL(NewAttribute(QString)), this, SLOT(NewAttribute(QString)), Qt::QueuedConnection );
      connect(this, SIGNAL(NewResultAttributes(QString)), form, SLOT(NewResultAttributes(QString)), Qt::QueuedConnection );
    }


    mv_type = MV_OCR;
}

/// ===========================================================================
///
/// ===========================================================================
MvOCR::~MvOCR()
{
    if(form)
    {
        delete form;
        form = nullptr;
    }
}

/// ===========================================================================
///
/// ===========================================================================
bool MvOCR::Exec(quint32 proc_id)
{

    this->Clear();
    tm_exec.start();

    //--------------- Protection Rect ----------------------//
    if( b_Busy )
    {
        emit( NewResult(false, tr("Busy") + QString::number(tm_exec.elapsed()) + " ms", proc_id ));
        return false;
    }
    b_Busy = true;



    if( ExtractRoi() == false )
    {
        b_Busy = false;
        return false;
    }

    QMetaObject::invokeMethod(&PO, "Exec", Qt::QueuedConnection, Q_ARG(cv::Mat, roi), Q_ARG(quint32, proc_id) );

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCR::ExecResult(const QImage &img, const QString &text, quint32 prid)
{

    b_Busy = false;

    extractedText = text;
    QString s = tr("Leitura: ") +  extractedText + (" em ") + QString::number(tm_exec.elapsed()) + " ms";

    b_approved = expectedText == text;

    if( bShowThreshold )
    {
        result_image = img;
    }

    this->update();

    if( extractedText.size() < expectedText.size() / 2 )
        emit( NewResult(false, tr("Não detectado! em ") + QString::number(tm_exec.elapsed()) + " ms", prid ));
    else
        emit( NewResult((expectedText == extractedText), s, prid) );
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCR::NewAttribute(const QString value)
{
    expectedText = table.value(value);
    attribute = value;
    form->SetExpectedText(expectedText.contains(" ")? expectedText.remove(" "): expectedText);
    PO.SetExpectedText(expectedText);
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCR::Clear()
{
    extractedText.clear();
    b_approved = true;
    result_image = QImage();

}

/// ===========================================================================
///
/// ===========================================================================
void MvOCR::Reset()
{
    white_filter = 0;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvOCR::ShowDialog(bool bEnable)
{
    if(pLayout == nullptr) return false;
    while( pLayout->count() )
    {
        QLayoutItem* l = pLayout->itemAt(0);
        QWidget* p = l->widget();
        pLayout->removeWidget(p);
        p->close();
    }
    pLayout->addWidget( form );
    form->showNormal();

    if(qApp->property("USE_DATABASE").toBool()){form->SetTableDataBase(table.keys());}

    if( bEnable ) SetLock( false );
    this->update();
    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCR::SetExpectedText(const QString &s)
{
    PO.SetExpectedText( s );
    expectedText = s;

    if(qApp->property("USE_DATABASE").toBool())
    {
         emit(NewResultAttributes(s));
    }

}

/// ===========================================================================
///
/// ===========================================================================
void MvOCR::SetWhiteFilterSize(int n)
{
    white_filter = n;
    PO.SetWhiteFilterSize( n );
}


/// ===========================================================================
///
/// ===========================================================================
void MvOCR::SetBlackAndWhite(const int n)
{
   b_black_And_white = n;
   PO.SetBlackAndWhite(n);
  // Debug(n);
   form->SetBackgroundStateTool(n);

}

/// ===========================================================================
///
/// ===========================================================================
int MvOCR::GetBlackAndWhite()
{
    return b_black_And_white;

}

/// ===========================================================================
///
/// ===========================================================================
void MvOCR::SetRotationStateTool(const bool &v)
{
    form->SetRotationStateTool(v);
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCR::ResetMove()
{

}


/// ===========================================================================
///
/// ===========================================================================
void MvOCR::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if((b_exec_on_move == true) &&(b_lock_angle == false) && b_locked == false)
    {
        QPen pn(QColor(253,233,10), 3);
        pn.setCosmetic( true );
        pn.setWidth( 3 );
        painter->setPen( pn );

        QPointF pointer_= this->boundingRect().topRight() - QPointF(4,-2);

        float end_y , start_y, end_x ,start_x;

        end_x = pointer_.rx() - 20 ;
        end_y = pointer_.ry() + 20;
        start_x= pointer_.rx() ;
        start_y= pointer_.ry();

        double angle = atan2 (end_y - start_y, end_x - start_x) + M_PI;
        double x1,x2,y1,y2;
        double arrow_lenght_ = -5;
        double arrow_degrees_= 60;

        x1 = end_x + arrow_lenght_ * cos(angle - arrow_degrees_);
        y1 = end_y + arrow_lenght_ * sin(angle - arrow_degrees_);
        x2 = end_x + arrow_lenght_ * cos(angle + arrow_degrees_);
        y2 = end_y + arrow_lenght_ * sin(angle + arrow_degrees_);

        painter->drawLine(this->boundingRect().topLeft() - QPointF(-4,-2), this->boundingRect().topRight() - QPointF(4,-2));
        painter->drawLine(pointer_,QPointF(x1,y1));

    }

    if( bShowThreshold )
    {
        painter->drawImage(this->boundingRect(), result_image);
    }

    if( b_approved == false )
    {
        QPen pn(QColor(255,0,0), 3);
        pn.setCosmetic( true );
        pn.setWidth( 3 );
        painter->setPen( pn );

        painter->drawRect( this->boundingRect().adjusted(4,4,-4,-4) );
    }

    MvTool::paint(painter, option, widget);
}




