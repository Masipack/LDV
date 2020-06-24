#include "mvocrpo.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "global_defines.h"
#include "util/systemsettings.h"
#include "util/fileutil.h"
#include "util/systemsettings.h"

//#include "AVLConverters/AVL_OpenCV.h"
//#include <AVL_Lite.h>
//#include <AVLConverters/AVL_QT.h>


using namespace std;
using namespace  cv;
//using namespace avl;


///// ===========================================================================
///// bool use_PRIN bool use_PRINTER;TER;
///// ===========================================================================
//static Mat get_hogdescriptor_visu(const Mat& color_origImg, vector<float>& descriptorValues, const Size & size )
//{
//    const int DIMX = size.width;
//    const int DIMY = size.height;
//    float zoomFac = 9;
//    Mat visu;
//    cv::resize(color_origImg, visu, Size(static_cast<int>(color_origImg.cols*zoomFac), static_cast<int>(color_origImg.rows*zoomFac) ) );

//    int cellSize        = 10;
//    int gradientBinSize = 9;
//    float radRangeForOneBin = static_cast<float>(CV_PI/static_cast<float>(gradientBinSize)); // dividing 180 into 9 bins, how large (in rad) is one bin?

//    // prepare data structure: 9 orientation / gradient strenghts for each cell
//    int cells_in_x_dir = DIMX / cellSize;
//    int cells_in_y_dir = DIMY / cellSize;
//    float*** gradientStrengths = new float**[cells_in_y_dir];
//    int** cellUpdateCounter   = new int*[cells_in_y_dir];
//    for (int y=0; y<cells_in_y_dir; y++)
//    {
//        gradientStrengths[y] = new float*[cells_in_x_dir];
//        cellUpdateCounter[y] = new int[cells_in_x_dir];
//        for (int x=0; x<cells_in_x_dir; x++)
//        {
//            gradientStrengths[y][x] = new float[gradientBinSize];
//            cellUpdateCounter[y][x] = 0;

//            for (int bin=0; bin<gradientBinSize; bin++)
//                gradientStrengths[y][x][bin] = 0.0;
//        }
//    }

//    // nr of blocks = nr of cells - 1
//    // since there is a new block on each cell (overlapping blocks!) but the last one
//    int blocks_in_x_dir = cells_in_x_dir - 1;
//    int blocks_in_y_dir = cells_in_y_dir - 1;

//    // compute gradient strengths per cell
//    int descriptorDataIdx = 0;
//    int cellx = 0;
//    int celly = 0;

//    for (int blockx=0; blockx<blocks_in_x_dir; blockx++)
//    { /* HOGDescr bool use_PRINTER;iptor hog(  Size(40,80),               // winSize
//                        Size(10,10),               // blocksize
//                        Size(5,5),                 // blockStride,
//                        Size(10,10),               // cellSize,
//                        9,                         // nbins,
//                        1,                         // derivAper,
//                        -1,                        // winSigma,
//                        HOGDescriptor::L2Hys,      // histogramNormType,
//                        0.2,                       // L2HysThresh,
//                        1,                         // gammal correction,
//                        64                         // nlevels=64
//                        );  */                       // Use signed gradients

//        for (int blocky=0; blocky<blocks_in_y_dir; blocky++)
//        {
//            // 4  bool use_PRINTER;cells per block ...
//            for (int cellNr=0; cellNr<4; cellNr++)
//            {
//                // compute corresponding cell nr
//                cellx = blockx;
//                celly = blocky;
//                if (cellNr==1) celly++;
//                if (cellNr==2) cellx++;
//                if (cellNr==3)
//                {
//                    cellx++;
//                    celly++;
//                }

//                for (int bin=0; bin<gradientBinSize; bin++)
//                {
//                    float gradientStrength = static_cast<float>(descriptorValues[descriptorDataIdx]);
//                    descriptorDataIdx++;

//                    gradientStrengths[celly][cellx][bin] += gradientStrength;

//                } // for (all bins)


//                // note: overlapping blocks lead to multiple updates of this sum!
//                // we therefore keep track how often a cell was updated,
//                // to compute average gradient strengths
//                cellUpdateCounter[celly][cellx]++;

//            } // for (all cells)


//        } // for (all block x pos)
//    } // for (all block y pos)


//    // compute average gradient strengths
//    for (celly=0; celly<cells_in_y_dir; celly++)
//    {
//        for (cellx=0; cellx<cells_in_x_dir; cellx++)
//        {

//            float NrUpdatesForThisCell = static_cast<float>(cellUpdateCounter[celly][cellx]);

//            // compute average gradient strenghts for each gradient bin direction
//            for (int bin=0; bin<gradientBinSize; bin++)
//            {
//                gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
//            }
//        }
//    }

//    // draw cells
//    for (celly=0; celly<cells_in_y_dir; celly++)
//    {
//        for (cellx=0; cellx<cells_in_x_dir; cellx++)
//        {
//            int drawX = cellx * cellSize;
//            int drawY = celly * cellSize;

//            int mx = drawX + cellSize/2;
//            int my = drawY + cellSize/2;

//            rectangle(visu, Point(static_cast<int>((drawX*zoomFac)), static_cast<int>(drawY*zoomFac)), Point(static_cast<int>((drawX+cellSize)*zoomFac), static_cast<int>((drawY+cellSize)*zoomFac)), Scalar(100,100,100), 1);

//            // draw in each cell all 9 gradient strengths
//            for (int bin=0; bin<gradientBinSize; bin++)
//            {
//                float currentGradStrength = gradientStrengths[celly][cellx][bin];

//                // no line to draw?
//                if (currentGradStrength ==0)
//                    continue;

//                float currRad = bin * radRangeForOneBin + radRangeForOneBin/2;

//                float dirVecX = cos( currRad );
//                float dirVecY = sin( currRad );
//                float maxVecLen = static_cast<float>(cellSize/2.f);
//                float scale = 2.5; // just a visualization scale, to see the lines better

//                // compute line coordinates
//                float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
//                float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
//                float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
//                float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

//                // draw gradient visualization
//                line(visu, Point(static_cast<int>((x1*zoomFac)),static_cast<int>(y1*zoomFac)), Point(static_cast<int>((x2*zoomFac)),static_cast<int>(y2*zoomFac)), Scalar(0,255,0), 1);

//            } // for (all bins)

//        } // for (cellx)
//    } // for (celly)


//    // don't forget to free memory allocated by helper data structures!
//    for (int y=0; y<cells_in_y_dir; y++)
//    {
//        for (int x=0; x<cells_in_x_dir; x++)
//        {
//            delete[] gradientStrengths[y][x];
//        }
//        delete[] gradientStrengths[y];
//        delete[] cellUpdateCounter[y];
//    }
//    delete[] gradientStrengths;
//    delete[] cellUpdateCounter;

//    return visu;

//}


/// ===========================================================================
///
/// ===========================================================================
MvOCRPO::MvOCRPO(QObject *parent) : QObject(parent),use_DARK(false)
{
    expectedText.clear();

    filterSizes << 0 << 3 << 5 << 7 << 9 << 11 << 13 << 15;

    white_filter = 0;

    b_black_And_white = 0;

    GetConfig(use_DARK  , "SYSTEM/USE_DARK",false);
    GetConfig(use_LASER , "SYSTEM/USE_LASER",false);

}

/// ===========================================================================
///
/// ===========================================================================
MvOCRPO::~MvOCRPO()
{

}

/// ===========================================================================
///
/// ===========================================================================
bool MvOCRPO::Init()
{
    svm_NUMBER          = cv::ml::SVM::create();
    svm_ALPHA_UCASE     = cv::ml::SVM::create();
    svm_ALPHA_LCASE     = cv::ml::SVM::create();
    svm_SYMBOL          = cv::ml::SVM::create();

    svm_NUMBER          = cv::ml::SVM::load(std::string("./data/fonts/NU.bin"));
    svm_ALPHA_UCASE     = cv::ml::SVM::load(std::string("./data/fonts/AU.bin"));
    svm_ALPHA_LCASE     = cv::ml::SVM::load(std::string("./data/fonts/AL.bin"));
    svm_SYMBOL          = cv::ml::SVM::load(std::string("./data/fonts/SY.bin"));

    hog =  HOGDescriptor (cv::Size(40,80), //winSize
                              cv::Size(10,10), //blocksize
                              cv::Size(5,5), //blockStride,
                              cv::Size(10,10), //cellSize,
                              9,      //nbins,
                              1,      //derivAper,
                             -1,     //winSigma,
                              HOGDescriptor::L2Hys,      //histogramNormType,
                              0.2,    //L2HysThresh,
                              1,      //gammal correction,
                              64     //nlevels=64
                               );     //Use signed gradients)

    return true;

}

/// ===========================================================================
///
/// ===========================================================================
void MvOCRPO::SetWhiteFilterSize(int n)
{
    white_filter = n;
}

/// ===========================================================================
///
/// ===========================================================================
QImage MvOCRPO::GetThresholdedImage()
{
    return th_roi;
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCRPO::SetBlackAndWhite(const int n)
{
      b_black_And_white = n;
}

/// ===========================================================================
///
/// ===========================================================================
void MvOCRPO::Exec(const cv::Mat &roi, quint32 proc_id)
{

    Clear();
    current_proc_id     = proc_id;

    Mat roi_local = roi.clone();
    Mat roi_local_th = roi.clone();


    try
    {

        GaussianBlur(roi_local, roi_local, Size(7,7),0,0);

        Mat kern = (Mat_<char>(5, 5) <<  0, 0, 0, 0, 0,
                                         0, 1, 1, 1, 0,
                                         0, 0, 1, 0, 0,
                                         0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0);

        filter2D(roi_local_th, roi_local_th, roi_local_th.depth(), kern);

     //   adaptiveThreshold(roi_local, roi_local_th, 255, ADAPTIVE_THRESH_GAUSSIAN_C, b_black_And_white == 1 ? THRESH_BINARY:THRESH_BINARY_INV, 29, -3);
       //  GaussianBlur(roi_local, roi_local, Size(5,5),2,2);

       threshold(roi_local, roi_local_th, 120, 255, b_black_And_white == 1 ? THRESH_BINARY|THRESH_OTSU: THRESH_BINARY_INV|THRESH_OTSU);

      //  GaussianBlur(roi_local, roi_local_th, Size(5,5),2,2);
//        Mat kern = (Mat_<char>(5, 5) <<  0, 0, 1, 0, 0,
//                                         0, 0, 1, 0, 0,
//                                         0, 0, 1, 0, 0,
//                                         0, 0, 1, 0, 0,
//                                         0, 0, 1, 0, 0);


    } catch (cv::Exception ex) {

           LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: FALHA NA EXTRAÇÃO DE CONTORNOS %1 ").arg(QString::fromStdString(ex.err)));
    }


//    namedWindow("Thresold", WINDOW_NORMAL );
//    imshow("Thresold", roi_local_th);

    int erode_sizeA  = 0; // white
    int dilate_sizeA = 0; // black

    erode_sizeA  = filterSizes[ white_filter ];


    Mat element2;

    if(erode_sizeA)
    {
        dilate_sizeA = erode_sizeA * 2 - 1;
    }

    if(use_LASER)
    {

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
    }
    else{

        if( erode_sizeA )
        {
            element2 = getStructuringElement( MORPH_ELLIPSE, Size(dilate_sizeA,dilate_sizeA) );
            dilate(roi_local_th,roi_local_th,element2);
        }
  }




    th_roi = QImage( roi_local_th.cols, roi_local_th.rows, QImage::Format_Grayscale8 );


    for( int i = 0; i < roi_local_th.rows; i++)
    {
        memcpy( &th_roi.bits()[ th_roi.bytesPerLine() * i ], roi_local_th.row(i).data, roi_local_th.cols * roi_local_th.channels() );
    }

    if( expectedText.size() == 0 )
    {
        emit( ExecResult(th_roi, extractedText, proc_id) );
        return;
    }

    qreal min_width = ((qreal)roi.cols / (qreal)expectedText.size()) * 0.1;

    qreal max_width= ((qreal)roi.cols / (qreal)expectedText.size()) * 1.2;

    qreal min_height = (qreal)roi.rows * 0.2;
    qreal max_height = (qreal)roi.rows * 1.4;

    std::vector< std::vector<cv::Point> >  contours;
    std::vector< Vec4i >                   hierarchy;


    findContours(roi_local_th, contours, hierarchy,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );

    Mat blobs = roi.clone();
    blobs.setTo( Scalar(0) );

    Rect CARACTER_SIZE = cv::boundingRect(contours[0]);

    for( uint i = 0; i < contours.size(); i=hierarchy[i][0] )
    {
        Rect br = cv::boundingRect(contours[i]);

        if( br.width  < 4  )                        continue;
        if( br.width  > 80 )                        continue;
        if(CARACTER_SIZE.area() < br.area()) CARACTER_SIZE = br;
    }

    for(unsigned int i = 0; i < contours.size(); i=hierarchy[i][0] )
    {

        Rect br = cv::boundingRect(contours[i]);

        if(use_DARK)
        {
            if(br.area()  < 20)                      continue;
            if(br.width   < 4)                       continue;
            if(br.width   > 40)                      continue;
            if(br.height  < 20)                      continue;
            if(br.height  > 80)                      continue;
        }else{


        //    5.33846
        //    64.0615
        //    30.4
        //    212.8

        if( br.height  < 30)                  continue;
        if( br.width   < qFloor(min_width)  ) continue;
        if( br.width   > qCeil(max_width)   ) continue;
        if( br.height  < qFloor(min_height) - 10) continue;
        if( br.height  > qCeil(max_height*0.5)  ) continue;

//        Debug(min_width)
//        Debug(max_width)
//        Debug(min_height)
//        Debug(max_height)



        }


       drawContours( blobs, contours, i, Scalar(255), -1);

       int k = hierarchy[i][2];
        while( k != -1  )
        {
         drawContours( blobs, contours, k, Scalar(0), -1);
            k = hierarchy[k][0];
        }

        Rect char_rect( Point(br.tl().x, br.tl().y), cv::Size(br.width, br.height));

        Mat test_char;

        if((CARACTER_SIZE.width  + br.tl().x < roi.cols) && (CARACTER_SIZE.height + br.tl().y < roi.rows))
        {
            char_rect = cv::Rect(Point(br.tl().x, br.tl().y), cv::Size(CARACTER_SIZE.width, CARACTER_SIZE.height ) );

            if((br.width <= CARACTER_SIZE.width*0.5) && ((br.tl().x-8) > 0))
            {
                char_rect = cv::Rect(Point(br.tl().x-8, br.tl().y), cv::Size(CARACTER_SIZE.width, CARACTER_SIZE.height ) );
            }
        }


        test_char = roi_local(char_rect);

        cv::resize(test_char, test_char, cv::Size(40,80),0,0,cv::INTER_CUBIC);

        std::vector<float> descriptors;


        hog.compute(test_char,descriptors);


        DESCRIPTOR desc;
        desc.rect        = QRectF( br.tl().x, br.tl().y, br.width, br.height );
        desc.mat         = Mat(1, static_cast<int>(descriptors.size()), CV_32FC1);

        for( unsigned int n = 0; n < descriptors.size(); n++ )
        {
            desc.mat.at<float>(0,static_cast<int>(n)) = descriptors.at(n);
        }

        char_descriptors.push_back( desc );
    }


//    namedWindow("blob", WINDOW_NORMAL );
//    imshow("blob", blobs);

    qSort(char_descriptors.begin(), char_descriptors.end(), DescriptorThanX); // TODO: ver a rotacao da tela

    QString mask = expectedText;
    mask.remove("\n");
    mask.remove(" ");

    qreal lastY = 0;

    if( char_descriptors.size() ) lastY = char_descriptors.at(0).rect.y();

    QString test;


    for( int i = 0; i < char_descriptors.size(); i++ )
    {
        if(i >= mask.size() ) break;


        DESCRIPTOR& current = char_descriptors[i];

        int predicted = -1;

        try
        {
            if(true == mask.at(i).isNumber() )
            {
                predicted = static_cast<int>(svm_NUMBER->predict(current.mat.clone()));
            }

            if(true == mask.at(i).isLetter() && mask.at(i).isUpper() )
            {
                predicted = static_cast<int>(svm_ALPHA_UCASE->predict(current.mat.clone()));
            }

            if(true == mask.at(i).isLetter() && mask.at(i).isLower() )
            {
                predicted = static_cast<int>(svm_ALPHA_LCASE->predict(current.mat.clone()));
            }

            if(true == mask.at(i).isPunct() )
            {
              predicted = static_cast<int>(svm_SYMBOL->predict(current.mat.clone()));

            }


           extractedText.append((predicted >= 0x20 && predicted <= 0x7E) ? QChar(static_cast<int>(predicted)): QChar('?'));


        } catch (cv::Exception ex) {
            extractedText.clear();
            LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: FALHA NA EXTRAÇÃO DO CARACTERE %1 ").arg(QString::fromStdString(ex.err)));

        }

        lastY = current.rect.y();
    }

    emit( ExecResult(th_roi, extractedText, proc_id) );

}

/// ===========================================================================
///
/// ===========================================================================
void MvOCRPO::Clear()
{
    th_roi = QImage();
    char_descriptors.clear();
    extractedText.clear();
}
