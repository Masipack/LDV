#include "mvocrconfig.h"

#include <QtCore>
#include <opencv2/highgui.hpp>
#include <QPainter>
#include <QGraphicsView>
#include <opencv2/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "mv/inspectionbuffer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include "opencv2/highgui.hpp"
#include "mv/PO/mvocrpo.h"


using namespace cv;
using namespace  std;

/// ===========================================================================
///
/// ===========================================================================
static bool ContourLessThanX(const MvOCRPO::DESCRIPTOR &t1, const MvOCRPO::DESCRIPTOR &t2)
{
    QPointF s1  = t1.rect.center();
    QPointF s2  = t2.rect.center();
    qreal h     = t1.rect.height();

    if( qAbs(s1.y() - s2.y()) < h )
    {
        return (s1.x() < s2.x());
    }

    return (s1.y() < s2.y());
}


/// ===========================================================================
///
/// ===========================================================================
MvOCRConfig::MvOCRConfig(const QRectF& rect, MvTool *parent) : MvAbstractTool(rect, parent)
{
    this->SetToolName( "CFG");

    Initialize();
   // GetParametersArea(MIN_HEIGTH,MAX_HEIGTH,MIN_WIDTH,MAX_WIDTH);

    BlackAndWhite = -1;

    filterSizes << 0 << 3 << 5 << 7 << 9 << 11 << 13 << 15;
    white_filter = 1;

}

/// ===========================================================================
///
/// ===========================================================================
MvOCRConfig::~MvOCRConfig()
{

}


/// ===========================================================================
///
/// ===========================================================================
void MvOCRConfig::Initialize()
{
    MAX_HEIGTH =0;
    MIN_HEIGTH =0;
    MAX_WIDTH  =0;
    MIN_WIDTH  =0;
    currentItem=-1;
    THRESHOLD  =133;
    DISTANCE   =255;

}

/// ===========================================================================
///
/// ===========================================================================
QImage MvOCRConfig::GetImage()
{
    if( this->scene() == nullptr ) return QImage();
    InspectionBuffer* pBuf = qobject_cast<InspectionBuffer*>(this->scene());
    return pBuf->ToolImage(mapRectToScene(this->boundingRect()));
}

/// ===========================================================================
///
/// ===========================================================================
bool MvOCRConfig::Exec(quint32 proc_id)
{
    this->Clear();

    emit( TemplateChanged() );

    if( ExtractRoiGray() == false )     return false;

    Mat roi_local = roi.clone();
    Mat roi_local_th = roi.clone();

    GaussianBlur(roi_local, roi_local, cv::Size(9,9),0,0);

    threshold(roi_local, roi_local_th, 0, 255, BlackAndWhite == 1 ? THRESH_BINARY + THRESH_OTSU: THRESH_BINARY_INV + THRESH_OTSU);


    int erode_sizeA  = 0; // white
    int dilate_sizeA = 0; // black

    erode_sizeA  = filterSizes[ white_filter ];


    Mat element2;

    if(erode_sizeA)
    {
        dilate_sizeA = erode_sizeA * 2 - 1;
    }

    if( dilate_sizeA )
    {
        element2 = getStructuringElement( MORPH_ELLIPSE, Size(erode_sizeA,erode_sizeA) );
        erode(roi_local_th, roi_local_th,element2 );
    }


    if( erode_sizeA )
    {
        element2 = getStructuringElement( MORPH_ELLIPSE, Size(dilate_sizeA,dilate_sizeA) );
        dilate(roi_local_th,roi_local_th,element2);
    }



    if( erode_sizeA )
    {
        element2 = getStructuringElement( MORPH_ELLIPSE, Size(erode_sizeA,erode_sizeA) );
        erode(roi_local_th,roi_local_th,element2);
    }


    vector< vector<cv::Point> >       contours;
    vector< Vec4i >                   hierarchy;

//    namedWindow("thresold", WINDOW_NORMAL );
//    imshow("thresold", roi_local_th);


    findContours(roi_local_th, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE );

    if( ExtractRoiGray() == false )     return false;

     QPointF top_left = this->boundingRect().topLeft();

     ExtractedCaracterHOG(contours,hierarchy,top_left) ;

     return true;
}


/// ===========================================================================
/// Extração do Contorno com HOG
/// ===========================================================================
bool MvOCRConfig::ExtractedCaracterHOG(const vector<vector<Point> > &contours, const vector<Vec4i> &hierarchy, const QPointF &TL)
{
    if(contours.size() <= 0) return false;

    if(contours_found.isEmpty()== false) return false;

    Mat blobs = roi.clone();
    blobs.setTo( Scalar(0) );

    Mat at;

    for( unsigned int i = 0; i < contours.size(); i = hierarchy[i][0] )
    {

        Rect br = cv::boundingRect(contours[i]);

//        if( br.width  < 26 )                         continue;
//        if( br.width  > 59  )                        continue;

//        if( br.height < 24   )                       continue;
//        if( br.height > 140  )                       continue;

        if( br.width  < 26 && br.height < 26  )      continue;
        if( br.width  > 40 && br.height < 40  )      continue;

        drawContours( blobs, contours, static_cast<int>(i), Scalar(255), -1);

        int k = hierarchy[i][2];
        while( k != -1  )
        {
            drawContours( blobs, contours, k, Scalar(0), -1);
            k = hierarchy[k][0];
        }

        Rect char_rect( Point(br.tl().x, br.tl().y), Size(br.width, br.height));

        Mat test_char = roi(char_rect);

        at= roi(char_rect);

        cv::resize(test_char, test_char, cv::Size(40,80),0,0,cv::INTER_CUBIC);

        vector<float> descriptors;

        HOGDescriptor hog (Size(40,80), //winSize
                                Size(10,10), //blocksize
                                Size(5,5), //blockStride,
                                Size(10,10), //cellSize,
                                9,      //nbins,
                                1,      //derivAper,
                               -1,     //winSigma,
                                HOGDescriptor::L2Hys,      //histogramNormType,
                                0.2,    //L2HysThresh,
                                1,      //gammal correction,
                                64     //nlevels=64
                                 );     //Use signed gradients)


        hog.compute(test_char,descriptors);

        AddItem(descriptors,br,TL);
    }

    sort(contours_found.begin(), contours_found.end(), ContourLessThanX);

    return true;

}

/// ===========================================================================
///  Adiciona um novo contorno
/// ===========================================================================
void MvOCRConfig::AddItem(const vector<float> &descriptors, const Rect &br,   const QPointF &top_left)
{

    MvOCRPO::DESCRIPTOR ct;
    ct.rect        = QRectF( br.tl().x + top_left.x(), br.tl().y + top_left.y(), br.width, br.height );
    ct.mat         = Mat(1,static_cast<int>(descriptors.size()), CV_32FC1);
    ct.character   = QChar();
    ct.descriptors = QVector<float>::fromStdVector(descriptors);

    for( unsigned int n = 0; n < descriptors.size(); n++ )
    {
       ct.mat.at<float>(0,static_cast<int>(n)) = descriptors.at(n);
    }


    if( qAbs(br.width - br.height) < (br.height * 0.8) )
        ct.image = GetImage().copy( QRect( QPoint(br.tl().x-2, br.tl().y-2), QSize(br.width+4, br.height+4) ) );
    else
        ct.image = GetImage().copy( QRect( QPoint(br.tl().x-2 - (qAbs(br.height-br.width) / 2), br.tl().y-2), QSize(br.height+4, br.height+4) ) );

    contours_found.push_back( ct  );
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCRConfig::Clear()
{
    contours_found.clear();
    currentItem = -1;
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCRConfig::Reset()
{

}

/// ===========================================================================
///
/// ===========================================================================
void MvOCRConfig::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pn(QColor(0,255,0, 128), 1);
    pn.setCosmetic( true );
    painter->setPen( pn );

    foreach ( MvOCRPO::DESCRIPTOR c, contours_found)
    {
        painter->drawRect( c.rect );
    }


    if( currentItem != -1 && currentItem < contours_found.size() )
    {
        pn.setColor( Qt::red );
        pn.setWidth(2);
        painter->setPen( pn );
        painter->drawRect( contours_found[currentItem].rect );
    }

    MvTool::paint(painter, option, widget);
}

/// ===========================================================================
/// Retorna o contorno selecionado
/// ===========================================================================
bool MvOCRConfig::GetItem(int index, MvOCRPO::DESCRIPTOR& dest)
{
    if( index < 0 || index >= contours_found.size() ) return false;

    dest = contours_found.at(index);

    return true;
}


/// ===========================================================================
///
/// ===========================================================================
MvOCRPO::DESCRIPTOR MvOCRConfig::GetCONTOUR(int index)
{
    return ( index > 0 || index <= contours_found.size() ) ? contours_found.at(index): MvOCRPO::DESCRIPTOR();
}

/// ===========================================================================
/// Set um Nome para contorno
/// ===========================================================================
bool MvOCRConfig::SetItem(int index, QChar c)
{
    if( index < 0 || index >= contours_found.size() ) return false;

    contours_found[index].character = c;

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
QVector<float> MvOCRConfig::GetDescripters(int index)
{
  return contours_found[index].descriptors;
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCRConfig::RemoteAt(int i)
{
   contours_found.removeAt(i);
}



