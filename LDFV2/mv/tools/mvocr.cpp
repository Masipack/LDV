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



    if( ExtractRoi() == false )             return false;


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
    QString s = "Leitura: " +  extractedText + " em " + QString::number(tm_exec.elapsed()) + " ms";

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

}

/// ===========================================================================
///
/// ==========================================================================
int MvOCR::GetBlackAndWhite()
{
    return b_black_And_white;

}


/// ===========================================================================
///
/// ===========================================================================
void MvOCR::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

/// ===========================================================================
///
/// ===========================================================================
double MvOCR::correlation(cv::Mat& image_1, cv::Mat& image_2)
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
