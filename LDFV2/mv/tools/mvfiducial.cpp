#include "mvfiducial.h"

#include <QtCore>
#include <opencv2/highgui.hpp>
#include <QPainter>
#include <QGraphicsView>
#include "global_defines.h"

using namespace cv;

/// ===========================================================================
///
/// ===========================================================================
QImage static Mat2QImage(const cv::Mat &mat)
{
    QImage img((uchar*)mat.data, mat.cols, mat.rows, QImage::Format_ARGB32);
    return img;
}

/// ===========================================================================
///
/// ===========================================================================
MvFiducial::MvFiducial(const QRectF& rect, MvTool *parent) : MvAbstractTool(rect, parent)
{
    form = new ParamsFiducial(this);
    b_approved = false;

    correlation_val     = 0.0;
    min_correlation     = 0.60;

    mv_type             = MV_FIDUCIAL;

    bMove               = false;
}

/// ===========================================================================
///
/// ===========================================================================
MvFiducial::~MvFiducial()
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
void MvFiducial::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    MvAbstractTool::mouseMoveEvent(event);

    samplingRect.setWidth(   this->boundingRect().width()  / 6 );
    samplingRect.setHeight(  this->boundingRect().height() / 2 );
    samplingRect.moveCenter( this->boundingRect().center() );

    if( ExtractRoi(samplingRect) == false )     return;
    if( roi.channels() != 1 ) cvtColor(roi, roi, COLOR_RGB2GRAY );
    roi.copyTo( img_template );

    bMove = true;
}


/// ===========================================================================
///
/// ===========================================================================
bool MvFiducial::Exec(quint32 proc_id)
{
    this->Clear();
    tm_exec.start();

    if( ExtractRoi() == false )             return false;

    if( roi.channels() != 1 ) cvtColor(roi, roi, COLOR_RGB2GRAY );
    if( ExtractRoiGray() == false )         return false;
    if( img_template.data == nullptr)       return false;
    if( roi.cols < img_template.cols)       return false;
    if( roi.rows < img_template.rows)       return false;
    if( roi.type() != img_template.type() ) return false;

    Mat result;

    /// Create the result matrix
    int result_cols = roi.cols;
    int result_rows = roi.rows;
    result.create( result_rows, result_cols, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( roi, img_template, result, TM_CCOEFF );

    double minVal; double maxVal; Point minLoc; Point maxLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );

    Mat found_rect = roi( Rect(maxLoc.x, maxLoc.y, img_template.cols, img_template.rows) );

//        namedWindow("FOUND", WINDOW_NORMAL);
//        imshow("FOUND", found_rect);
//        namedWindow("TEMPLATE", WINDOW_NORMAL);
//        imshow("TEMPLATE", img_template);

    correlation_val = correlation( found_rect, img_template );

    templatePos = QPointF(maxLoc.x, maxLoc.y) + QPointF((qreal)img_template.cols*0.5, (qreal)img_template.rows*0.5) + this->boundingRect().topLeft();

    QString s = "Leitura: " +   QString("%1%").arg(correlation_val, 2, 'f', 2, QChar('0')) + " em " + QString::number(tm_exec.elapsed()) + " ms";


    if( correlation_val < min_correlation )
     {
         templatePos = QPointF();
         b_approved = false;
         emit( NewResult(false,s, proc_id) );
         return false;
     }

    this->update();

    emit( NewResult(true, s, proc_id) );

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
QPointF MvFiducial::GetDistance()
{
    if( templatePos.isNull() ) return QPointF(0,0);
    return templatePos - mapFromScene(configPos);
}

/// ===========================================================================
///
/// ===========================================================================
void MvFiducial::SetMinimumSimilarity(int n)
{
    min_correlation = qreal(n) * 0.01;
}

/// ===========================================================================
///
/// ===========================================================================
void MvFiducial::SetMinimumRegion(int n)
{
    min_per_region = qreal(n) * 0.01;

    QRectF resize_tool;

    resize_tool = this->boundingRect();
    resize_tool.adjust(0,0, resize_tool.width() * min_per_region, 0  );
    setIncrementSizeX(resize_tool);
    min_per_region_old = min_per_region;


}


/// ===========================================================================
///
/// ===========================================================================
void MvFiducial::LearnCurrentTemplate()
{
    samplingRect.setWidth(   this->boundingRect().width()  / 6 );
    samplingRect.setHeight(  this->boundingRect().height() / 2 );
    samplingRect.moveCenter( this->boundingRect().center() );

    if( ExtractRoi(samplingRect) == false )     return;
    if( roi.channels() != 1 ) cvtColor(roi, roi, COLOR_RGB2GRAY );
    roi.copyTo( img_template );

    bMove = true;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvFiducial::GetImageTemplate(QByteArray &dest, QSize& sz_dest)
{
    if( img_template.data == nullptr ) return false;

    sz_dest = QSize(img_template.cols,img_template.rows);
    dest = QByteArray((const char*)img_template.data, img_template.rows * img_template.cols);

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvFiducial::SetImageTemplate(const QByteArray &src, QSize sz)
{
    img_template = cv::Mat( cv::Size(sz.width(), sz.height()) , CV_8UC1, (void*)src.data() ).clone();

//    imshow("A",img_template);
}

/// ===========================================================================
///
/// ===========================================================================
void MvFiducial::Clear()
{
    //bMove      = false;
    b_approved   = true;
}

/// ===========================================================================
///
/// ===========================================================================
void MvFiducial::Reset()
{
}

/// ===========================================================================
///
/// ===========================================================================
bool MvFiducial::ShowDialog(bool bEnable)
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

    if( bEnable == false ) form->SetRunMode();
    SetLock( !bEnable );

    this->update();
    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void MvFiducial::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    samplingRect.setWidth(   this->boundingRect().width()  / 6 );
    samplingRect.setHeight(  this->boundingRect().height() / 2 );
    samplingRect.moveCenter( this->boundingRect().center() );

    if( bMove == true )
    {
        QPen pn(QColor(255,0,0), 3);
        pn.setCosmetic( true );
        painter->setPen( pn );

        painter->drawRect(samplingRect);

        pn.setColor(QColor(0,0,0));
        pn.setCosmetic( true );
        pn.setWidth(1);
        painter->setPen( pn );

        painter->drawRect(samplingRect);
    }
    else
    {
        if( templatePos.isNull() == false  )
        {
            QPen pn(QColor(0,255,0,128), 1);
            pn.setCosmetic( true );
            painter->setPen( pn );

            painter->drawEllipse( mapFromScene(configPos), 10, 10 );
            QString txt = QString("%1%").arg(qFloor(correlation_val*100.0));
            painter->drawText( QPointF(10,20) + boundingRect().topLeft(), txt );

            pn.setColor(QColor(252, 233, 79));
            pn.setWidth(3);
            pn.setCosmetic( true );
            painter->setPen( pn );

            painter->drawLine( templatePos - QPointF(cfg_handle_size, 0), templatePos + QPointF(cfg_handle_size, 0) );
            painter->drawLine( templatePos - QPointF(0, cfg_handle_size), templatePos + QPointF(0, cfg_handle_size) );


            pn.setColor(QColor(0,0,0));
            pn.setCosmetic( true );
            pn.setWidth(1);
            painter->setPen( pn );

            painter->drawLine( templatePos - QPointF(cfg_handle_size, 0), templatePos + QPointF(cfg_handle_size, 0) );
            painter->drawLine( templatePos - QPointF(0, cfg_handle_size), templatePos + QPointF(0, cfg_handle_size) );
        }
        else
        {
            QPen pn(QColor(0,255,0,128), 1);
            pn.setCosmetic( true );
            painter->setPen( pn );

            QString txt = QString("%1% > %2%").arg(qFloor(correlation_val*100.0)).arg(qFloor(min_correlation*100.0));
            painter->drawText( QPointF(10,20) + boundingRect().topLeft(), txt );
        }
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

/// ===========================================================================
///
/// ===========================================================================
double MvFiducial::correlation(cv::Mat& image_1, cv::Mat& image_2)
{
    // convert data-type to "float"
    cv::Mat im_float_1;
    image_1.convertTo(im_float_1, CV_32F);
    cv::Mat im_float_2;
    image_2.convertTo(im_float_2, CV_32F);

    int n_pixels = im_float_1.rows * im_float_1.cols;

    // Compute mean and standard deviation of both images
    cv::Scalar im1_Mean, im1_Std, im2_Mean, im2_Std;
    meanStdDev(im_float_1, im1_Mean, im1_Std);
    meanStdDev(im_float_2, im2_Mean, im2_Std);

    // Compute covariance and correlation coefficient
    double covar = (im_float_1 - im1_Mean).dot(im_float_2 - im2_Mean) / n_pixels;
    double correl = covar / (im1_Std[0] * im2_Std[0]);

    return correl;
}
