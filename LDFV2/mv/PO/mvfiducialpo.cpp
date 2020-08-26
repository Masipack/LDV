#include "mvfiducialpo.h"

#include "opencv2/core.hpp"
#include "opencv2/core/ocl.hpp"
#include <opencv2/highgui.hpp>
#include  "opencv2/imgproc.hpp"

using namespace cv;


/// ===========================================================================
///
/// ===========================================================================
MvFIDUCIALPO::MvFIDUCIALPO(QObject *parent) : QObject(parent)
{

}

MvFIDUCIALPO::~MvFIDUCIALPO()
{

}

/// ===========================================================================
///
/// ===========================================================================
void MvFIDUCIALPO::Exec(const cv::Mat &roi, quint32 proc_id)
{
    Mat result;
    QPointF  templatePos;


        namedWindow("Template", WINDOW_NORMAL );
        imshow("Template", mask);

        namedWindow("Roi", WINDOW_NORMAL );
        imshow("Roi", roi);
    /// Create the result matrix
    int result_cols = roi.cols;
    int result_rows = roi.rows;

    result.create( result_rows, result_cols, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( roi, mask,result, TM_CCOEFF);

    double minVal; double maxVal; Point minLoc; Point maxLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );

    Mat found_rect = roi( Rect(maxLoc.x, maxLoc.y, mask.cols, mask.rows) );

    templatePos = QPointF(maxLoc.x, maxLoc.y) + QPointF((qreal)mask.cols*0.5, (qreal)mask.rows*0.5) + startPosition;

    Mat template_mask = mask.clone();

    namedWindow("Result", WINDOW_NORMAL );
    imshow("Result", result);

    emit( ExecResult(correlation( found_rect, template_mask),templatePos, proc_id) );

    result.release();

}

/// ===========================================================================
///
/// ===========================================================================
double MvFIDUCIALPO::correlation(cv::Mat &image_1, cv::Mat &image_2)
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
