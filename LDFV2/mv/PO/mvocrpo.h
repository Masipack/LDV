#ifndef MVOCRPO_H
#define MVOCRPO_H

#include <QObject>
#include <QtCore>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/objdetect.hpp>

class MvOCRPO : public QObject
{
    Q_OBJECT
public:
    explicit MvOCRPO(QObject *parent = nullptr);
    virtual ~MvOCRPO();

    void Clear();
    bool Init();
    QString GetResult() { return extractedText; }
    void SetExpectedText(const QString& expected_text) { expectedText = expected_text; }

    void SetWhiteFilterSize(int n);
    QImage GetThresholdedImage();
    void SetBlackAndWhite(const int n);

signals:
    void ExecResult(const QImage& img, const QString& text, quint32 prid);

public slots:
    void Exec(const cv::Mat& roi, quint32 proc_id);

private:
    int white_filter;
    QList<int> filterSizes;

    int  b_black_And_white;
    bool use_DARK;


protected:
    cv::Ptr<cv::ml::SVM>    svm_NUMBER;
    cv::Ptr<cv::ml::SVM>    svm_ALPHA_UCASE;
    cv::Ptr<cv::ml::SVM>    svm_ALPHA_LCASE;
    cv::Ptr<cv::ml::SVM>    svm_SYMBOL;

    quint32                 current_proc_id;
    QString                 expectedText;
    QString                 extractedText;

    cv::HOGDescriptor       hog;

    QImage                  th_roi;

    struct DESCRIPTOR
    {
        QRectF          rect;
        cv::Mat         mat;
        QChar           character;
        QImage          image;
        QVector<float>  descriptors;
    };

    QVector<DESCRIPTOR> char_descriptors;

    static bool DescriptorThanX(const DESCRIPTOR &s1, const DESCRIPTOR &s2)
    {
        QPointF A   = s1.rect.center();
        QPointF B   = s2.rect.center();
        qreal h     = qMax(s1.rect.height(), s2.rect.height()) * 0.5;

        if( qAbs(A.y() - B.y()) <  h ) return (A.x() < B.x());
        return (A.y() < B.y());
    }

};

#endif // MVOCRPO_H
