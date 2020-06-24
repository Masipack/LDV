#ifndef MANEGERLIBRARY_H
#define MANEGERLIBRARY_H

#include "QObject"

#include "util/dlginfo.h"


#include "mv/PO/mvocrpo.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/ml/ml.hpp>

using namespace  cv;

class ManegerLibrary:public QObject
{
    Q_OBJECT
public:
    ManegerLibrary(QObject *parent = 0);
    virtual ~ManegerLibrary();

    bool Train();
    bool SaveDictionary(const QString& fname);
    bool LoadDictionary();
    void SetItem(const MvOCRPO::DESCRIPTOR contours);
    void RemoveItem(const int current_index);

    QList<MvOCRPO::DESCRIPTOR>GetLibrary();

    bool GetItem(const int current_index,MvOCRPO::DESCRIPTOR& contours);

protected:
    Mat ExtraiContornos(QList<MvOCRPO::DESCRIPTOR> contours_found);
    Mat ExtraiLabels();
    void Clear();
    void Extracted_Dictionary();
    bool CheckEqualAllElement(QList<MvOCRPO::DESCRIPTOR> contours_found);

private:

        QList<MvOCRPO::DESCRIPTOR>  contours_found;
        QList<MvOCRPO::DESCRIPTOR>  contours_model;
        QList<MvOCRPO::DESCRIPTOR>  contours_found_Simbol;
        QList<MvOCRPO::DESCRIPTOR>  contours_found_Numbers;
        QList<MvOCRPO::DESCRIPTOR>  contours_found_LetterLower;
        QList<MvOCRPO::DESCRIPTOR>  contours_found_LetterUpper;

        QVector<int>    labels;

        DlgInfo         dlg;
};

#endif // MANEGERLIBRARY_H
