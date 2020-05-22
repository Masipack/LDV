#include "mvdatamatrix.h"

#include <QtCore>
#include <opencv2/highgui.hpp>
#include <QPainter>
#include <QGraphicsView>
#include "global_defines.h"
#include "util/sys_log.h"
#include "util/systemsettings.h"

#include "AVLConverters/AVL_OpenCV.h"
#include <AVL.h>
#include <AVLConverters/AVL_QT.h>
#include <ThirdPartySdk.h>

#define USE_MWPARSER    false
/* Parser */
/*
 *   Set the desired parser type
 *   Available options:
 *       MWP_PARSER_MASK_NONE
 *       MWP_PARSER_MASK_IUID
 *       MWP_PARSER_MASK_ISBT
 *       MWP_PARSER_MASK_AAMVA
 *       MWP_PARSER_MASK_HIBC
 *       MWP_PARSER_MASK_SCM
 */
#define MWPARSER_MASK   MWP_PARSER_MASK_AAMVA


#include "BarcodeScanner.h"

#if USE_MWPARSER
    #include "MWParser.h"
#endif

#include "util/MWResult.h"

#include "dmtx.h"

using namespace cv;
using namespace avl;


/// ===========================================================================
///
/// ===========================================================================
static QImage convert16uc3(const cv::Mat& source)
{
  quint8* pSource = (quint8*) source.data;
  int pixelCounts = source.cols * source.rows;

  QImage dest(source.cols, source.rows, QImage::Format_RGB32);

  char* pDest = (char*) dest.bits();

  for (int i = 0; i < pixelCounts; i++)
  {
    *(pDest++) = *(pSource+2);    // B
    *(pDest++) = *(pSource+1);    // G
    *(pDest++) = *(pSource+0);    // R
    *(pDest++) = 0;               // Alpha
    pSource+=3;
  }

  return dest.rgbSwapped();
}

static QImage convertToGrayScale(const QImage &srcImage) {
     // Convert to 32bit pixel format
     QImage dstImage = srcImage.convertToFormat(srcImage.hasAlphaChannel() ?
              QImage::Format_ARGB32 : QImage::Format_RGB32);

     unsigned int *data = (unsigned int*)dstImage.bits();
     int pixelCount = dstImage.width() * dstImage.height();

     // Convert each pixel to grayscale
     for(int i = 0; i < pixelCount; ++i) {
        int val = qGray(*data);
        *data = qRgba(val, val, val, qAlpha(*data));
        ++data;
     }

     return dstImage;
  }


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
MvDataMatrix::MvDataMatrix(const QRectF& rect, MvTool *parent) : MvAbstractTool(rect, parent)
{
    form = new  ParamsDataMatrix(this);
    mv_type = MV_DATAMATRIX;

    Init();
}

/// ===========================================================================
///
/// ===========================================================================
MvDataMatrix::~MvDataMatrix()
{
}

/// ===========================================================================
///
/// ===========================================================================
bool MvDataMatrix::Exec(quint32 proc_id)
{

    tm_exec.start();

    this->Clear();

    if( ExtractRoiGray() == false ) return false;

///---------------------------------------------------Cognex ---------------------------------------------------///

//    Init();

//    uint8_t * p_data = NULL;

//    int resultval = MWB_scanGrayscaleImage(roi.data, roi.rows, roi.cols, &p_data);

//    Debug(resultval)

//    if (resultval > 0)
//    {
//              MWResults *mwResults = new MWResults(p_data);
//              free(p_data);
//              MWResult *mwResult = NULL;

//              if (mwResults != NULL)
//              {
//                  mwResult =  mwResults->getResults().back();
//                  if (mwResult)
//                  {

//                      if (mwResult->locationPoints)
//                      {
//                           decodedObject obj;

//                           obj.type = QString(mwResult->getTypeName().c_str());
//                           obj.data = QString(mwResult->text.c_str());

//                           obj.location.append( cv::Point( mwResult->locationPoints->p1->x, mwResult->locationPoints->p1->y));
//                           obj.location.append( cv::Point( mwResult->locationPoints->p2->x, mwResult->locationPoints->p2->y));
//                           obj.location.append( cv::Point( mwResult->locationPoints->p3->x, mwResult->locationPoints->p3->y));
//                           obj.location.append( cv::Point( mwResult->locationPoints->p4->x, mwResult->locationPoints->p4->y));

//                           decodedObjects.push_back(obj);

//                      }
//                  }
//              }

//        delete mwResults;
//      }

//    if(decodedObjects.data()!= 0)
//    {
//        if(decodedObjects.size() > 0 ) extractedText   = decodedObjects[0].data;
//        QString s = "Leitura: " +  extractedText + " em " + QString::number(tm_exec.elapsed()) + " ms";
//        this->update();
//        emit(NewResult((expectedText == extractedText), s, proc_id) );
//    }
//    else{
//        emit( NewResult(false, tr("Não detectado! em ") + QString::number(tm_exec.elapsed()) + " ms", proc_id) );
//    }

    ///---------------------------------------------------Cognex ---------------------------------------------------///

    Mat im = roi.clone();



    try
    {
            Image output;

            DataMatrixDetectionParams detectionParams;
            DataMatrixCodeParams      codeParams;

            // Use dark polarity, i.e. detect black on white.
            codeParams.polarity = avl::Polarity::Any;

            // Try to detect even with a higher than default slant.
            codeParams.maxSlant = 50.0f;

            // Prepare buffer objects for output parameters.
            atl::Conditional<avl::DataCode> code;
            atl::Array<avl::Path> candidates;

            avl::CVMatToAvlImage(im,output);

            ReadSingleDataMatrixCode
                            (
                                output,
                                atl::NIL,
                                atl::NIL,
                                codeParams,
                                detectionParams,
                                code,
                                candidates,
                                atl::NIL
                            );

        // qDebug() << QString::fromUtf16(code.Get().text.CStr16());

            if( code.HasValue() )
            {
                   extractedText = QString::fromUtf16(code.Get().text.CStr16());
                   QString s = "Leitura: " +  extractedText + " em " + QString::number(tm_exec.elapsed()) + " ms";
                   this->update();
                   emit( NewResult((expectedText == extractedText), s, proc_id) );
               }
               else
               {
                   emit( NewResult(false, tr("Não detectado! em ") + QString::number(tm_exec.elapsed()) + " ms", proc_id) );
               }

    }
    catch(Exception ex)
    {
         LOG(LOG_ERROR_TYPE, QObject::tr("Prodtuo não licenciado") );
         return false;
    }

    return true;
}


/// ===========================================================================
///
/// ===========================================================================
void MvDataMatrix::Clear()
{
    extractedText.clear();
    if(!decodedObjects.isEmpty()) decodedObjects.clear();
}

/// ===========================================================================
///
/// ===========================================================================
void MvDataMatrix::Reset()
{
}

/// ===========================================================================
///
/// ===========================================================================
bool MvDataMatrix::ShowDialog(bool bEnable)
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
void MvDataMatrix::SetExpectedText(const QString &s)
{
    expectedText = s;
}

/// ===========================================================================
///
/// ===========================================================================
void MvDataMatrix::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

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
///
/// ===========================================================================
void MvDataMatrix::Init()
{
    QString license ;

    GetConfig(license, "SYSTEM/LICENSE", QString("ttyS0"));

    int registerResult = MWB_registerSDK(license.toUtf8());

     switch (registerResult) {
         case MWB_RTREG_OK:
             LOG(LOG_INFO_TYPE, QObject::tr("INFO       : %1 %2").arg("Registration OK\n"));
             break;
         case MWB_RTREG_INVALID_KEY:
             LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("Registration Invalid Key\n"));
             break;
         case MWB_RTREG_INVALID_CHECKSUM:
             LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("Registration Invalid Checksum\n"));
             break;
         case MWB_RTREG_INVALID_APPLICATION:
             LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("Registration Invalid Application\n"));
             break;
         case MWB_RTREG_INVALID_SDK_VERSION:
             LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("Registration Invalid SDK Version\n"));
             break;
         case MWB_RTREG_INVALID_KEY_VERSION:
             LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("Registration Invalid Key Version\n"));
             break;
         case MWB_RTREG_INVALID_PLATFORM:
             LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("Registration Invalid Platform\n"));
             break;
         case MWB_RTREG_KEY_EXPIRED:
             LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("Registration Key Expired\n"));
             break;
         default:
             LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("Registration Unknown Error\n"));
             break;
     }

     MWB_setResultType(MWB_RESULT_TYPE_MW);
     MWB_setResultType(MWB_RESULT_TYPE_MW);
     MWB_setDirection(MWB_SCANDIRECTION_HORIZONTAL | MWB_SCANDIRECTION_VERTICAL);
     MWB_setScanningRect(MWB_CODE_MASK_PDF, 0, 00, 100, 100);

     if (MWB_setActiveCodes(MWB_CODE_MASK_25    |
                               MWB_CODE_MASK_39     |
                               MWB_CODE_MASK_93     |
                               MWB_CODE_MASK_128    |
                               MWB_CODE_MASK_AZTEC  |
                               MWB_CODE_MASK_DM     |
                               MWB_CODE_MASK_EANUPC |
                               MWB_CODE_MASK_PDF    |
                               MWB_CODE_MASK_QR     |
                               MWB_CODE_MASK_CODABAR|
                               MWB_CODE_MASK_DOTCODE|
                               MWB_CODE_MASK_11     |
                               MWB_CODE_MASK_MSI    |
                               MWB_CODE_MASK_MAXICODE    |
                               MWB_CODE_MASK_POSTAL |
                               MWB_CODE_MASK_RSS)
                                    != MWB_RT_OK) {
          LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("MWB_setActiveSubcodes for RSS failed\n"));

        }

     /* (optional) configure flags for Code39 */
    if (MWB_setFlags(MWB_CODE_MASK_39, MWB_CFG_CODE39_EXTENDED_MODE) != MWB_RT_OK)
    {
      LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: %1 %2").arg("MWB_setFlags for Code39 failed\n"));
    }
}
