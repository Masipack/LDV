#include "mvbarcode.h"

#include <QtCore>
#include <opencv2/highgui.hpp>
#include <QPainter>
#include <QGraphicsView>
#include "global_defines.h"


#include "zbar.h"


using namespace cv;
using namespace zbar;
using namespace std;

/// ===========================================================================
///
/// ===========================================================================
MvBarcode::MvBarcode(const QRectF& rect, MvTool *parent) : MvAbstractTool(rect, parent)
{
    form = new  ParamsBarcode(this);


    b_pharmacode     = false;
    b_pharmacode_inv = false;

    mv_type = MV_BARCODE;
}

/// ===========================================================================
///
/// ===========================================================================
MvBarcode::~MvBarcode()
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
bool MvBarcode::Exec(quint32 proc_id)
{
    this->Clear();

    tm_exec.start();

    if( ExtractRoiGray() == false ) return false;

    decode(roi, decodedObjects);

    if( roi.cols < 20 || roi.rows < 8 ) return false;

    normalize( roi, roi, 0, 255,NORM_MINMAX);

    cv::Rect r1(0           ,0 , 4,4);
    cv::Rect r2(roi.cols - 4,0 , 4,4);

    Mat L = roi(r1);
    Mat R = roi(r2);



    qreal mean = (cv::mean(L)[0] + cv::mean(R)[0]) * 0.5;

    if( mean < 128.0 )
    {
        roi = 255 - roi;
    }

    Mat roi_local = roi;
    unsigned int newHints = 0;



    if( bShowThreshold )
    {
        result_image = QImage( roi_local.cols, roi_local.rows, QImage::Format_Grayscale8 );
        for( int i = 0; i < roi_local.rows; i++)
        {
            memcpy( &result_image.bits()[ result_image.bytesPerLine() * i ], roi_local.row(i).data, roi_local.cols * roi_local.channels() );
        }
    }

    if( b_pharmacode )
    {
        return PharmaCodeRead(roi_local, proc_id);
    }

    //------------------- Find and decode barcodes and QR codes -------------//

    resize(roi, roi, cv::Size(roi.cols ,roi.rows * 0.50), 0, 0, INTER_LINEAR);




    if(decodedObjects.data()!= 0)
    {
        if(decodedObjects.size() > 0 ) extractedText   = decodedObjects[0].data;
        QString s = "Leitura: " +  extractedText + " em " + QString::number(tm_exec.elapsed()) + " ms";
        this->update();
        emit( NewResult((expectedText == extractedText), s, proc_id) );
    }
    else{
        emit( NewResult(false, tr("Não detectado! em ") + QString::number(tm_exec.elapsed()) + " ms", proc_id) );
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvBarcode::PharmaCodeRead(Mat &roi_local, quint32 proc_id)
{
    QVector<int> bar_sizes;
    qreal last = -1.0;
    int n = 0;
    int n_max = 0;

    for( int i = 0; i < roi_local.cols; i++ )
    {
        qreal v = cv::mean(roi_local.col(i))[0];
        if( last == -1.0 )
        {
            last = v;
            continue;
        }

        if( last < 128.0 && v >= 128.0 )
        {
            n++;
            if( n > 2 ) bar_sizes.push_back(n);
            n_max = qMax(n_max, n);
            n = 0;
            last = v;
            continue;
        }

        if( v < 128.0 )
        {
            n++;
            last = v;
            continue;
        }
    }


    quint32 v = 0;
    if( b_pharmacode_inv )
    {
        int n = (bar_sizes.size()-1);
        for(int i = 0; i < bar_sizes.size(); i++)
        {
            bar_sizes[i] /= (n_max-1);
            v += 1 << ((n-i) + bar_sizes[i]);
        }
    }
    else
    {
        for(int i = 0; i < bar_sizes.size(); i++)
        {
            bar_sizes[i] /= (n_max-1);
            v += 1 << i + bar_sizes[i];
        }
    }
    extractedText += QString::number( v );

    if( bar_sizes.size() > 3 )
    {
        QString s = "Leitura: " +  extractedText + " em " + QString::number(tm_exec.elapsed()) + " ms";
        this->update();
        emit( NewResult((expectedText == extractedText), s, proc_id) );
    }
    else
    {
        emit( NewResult(false, tr("Não detectado! em ") + QString::number(tm_exec.elapsed()) + " ms", proc_id) );
    }

    return true;
}


/// ===========================================================================
///
/// ===========================================================================
void MvBarcode::Clear()
{
    extractedText.clear();
    if(!decodedObjects.isEmpty()) decodedObjects.clear();
}

/// ===========================================================================
///
/// ===========================================================================
void MvBarcode::Reset()
{
}

/// ===========================================================================
///
/// ===========================================================================
bool MvBarcode::ShowDialog(bool bEnable)
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
void MvBarcode::SetExpectedText(const QString &s)
{
    expectedText = s;
}

/// ===========================================================================
///
/// ===========================================================================
void MvBarcode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if( bShowThreshold )
    {
        painter->drawImage(this->boundingRect(), result_image);
    }

    QPen pn(QColor(0,255,0,128), 1);
    pn.setCosmetic( true );
    pn.setWidth(2);
    painter->setPen( pn );

    for(int i = 0; i < decodedObjects.size(); i++)
     {
       vector<Point> points = decodedObjects[i].location.toStdVector();
       vector<Point> hull;

       // If the points do not form a quad, find convex hull
       if(points.size() > 4)
         convexHull(points, hull);
       else
         hull = points;

       // Number of points in the convex hull
       int n = hull.size();

       for(int j = 0; j < n; j++)
       {
         QPointF start = QPointF(hull[j].x,hull[j].y);
         QPointF end   = QPointF(hull[ (j+1) % n].x,hull[ (j+1) % n].y);


         painter->drawLine(start, end);
       }

   }

   MvTool::paint(painter, option, widget);
}

/// ===========================================================================
///  Find and decode barcodes and QR codes
/// ===========================================================================
void MvBarcode::decode(Mat &im, QVector<decodedObject>&decodedObjects)
{

  // Create zbar scanner
  ImageScanner scanner;

  // Configure scanner
  scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

  // Wrap image data in a zbar image
  Image image(im.cols, im.rows, "Y800", (uchar *)im.data, im.cols * im.rows);

  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);

  // Print results
  for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
  {
    decodedObject obj;

    obj.type = QString(symbol->get_type_name().c_str());
    obj.data = QString(symbol->get_data().c_str());

//    qDebug() << Q_FUNC_INFO << __LINE__ << obj.type ;
//    qDebug() << Q_FUNC_INFO << __LINE__ << obj.data ;


    // Obtain location
    for(int i = 0; i< symbol->get_location_size(); i++)
    {
      obj.location.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
    }

    decodedObjects.push_back(obj);
  }
}
