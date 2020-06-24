#include "manegerlibrary.h"
#include "util/manegerlibraryUtils.h"

using namespace cv::ml;


/// ===========================================================================
///
/// ===========================================================================
static QVector<int> caracteres(QList<MvOCRPO::DESCRIPTOR> contours_found)
{
    QVector<int> result;

    foreach(MvOCRPO::DESCRIPTOR contours, contours_found)
    {
        result.push_front(contours.character.toLatin1());
    }

    return result;
}


/// ===========================================================================
///
/// ===========================================================================
ManegerLibrary::ManegerLibrary(QObject *parent)
{
  Clear();
}

/// ===========================================================================
///
/// ===========================================================================
ManegerLibrary::~ManegerLibrary()
{

}

/// ===========================================================================
///
/// ===========================================================================
bool ManegerLibrary::Train()
{
    Extracted_Dictionary();

    QString fnameNumbers = "./data/fonts/NU.bin";
    QString fnameSimbol  = "./data/fonts/SY.bin";
    QString fnameLetter_lower = "./data/fonts/AL.bin";
    QString fnameLetter_upper = "./data/fonts/AU.bin";


    dlg.SetMessage(DlgInfo::IT_WARNING, tr(" Aguarde, salvando uma nova biblioteca..... !"));
    dlg.SetVisible(false);
    dlg.show();

    for (int var = 0; var < 100; ++var) {
        qApp->processEvents(QEventLoop::ProcessEventsFlag::AllEvents);
        QThread::msleep(1);
    }

    // Set up SVM's parameters
    Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::LINEAR);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, (int)1e7, 1e-6));
    //svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 10000, FLT_EPSILON));
    svm->setC( 1.5 );


    try
    {
          if(!contours_found_Numbers.isEmpty() && (CheckEqualAllElement(contours_found_Numbers)==false) && (contours_found_Numbers.size() > 2))
          {
              Mat contornos_Numbers =ExtraiContornos(contours_found_Numbers).clone();

              // Train the SVM with given parameters
              Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(contornos_Numbers, cv::ml::ROW_SAMPLE, ExtraiLabels());
              svm->train(td);
              svm->save(fnameNumbers.toLatin1().data());

          }


    }
    catch (cv::Exception ex)
    {
        LOG( LOG_ERROR_TYPE, tr("Erro %1  na tentativa de criar um  novo dicionario de números").arg(QString::fromStdString(ex.msg)));
        return false;
    }

    try
    {
        if(!contours_found_Simbol.isEmpty() && (CheckEqualAllElement(contours_found_Simbol)==false) && (contours_found_Simbol.size() > 2) )
        {
            Mat contornos_Simbol = ExtraiContornos(contours_found_Simbol).clone();
            // Train the SVM with given parameters
            Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(contornos_Simbol, cv::ml::ROW_SAMPLE, ExtraiLabels());
            svm->train(td);
            svm->save(fnameSimbol.toLatin1().data());
        }

    }
    catch (cv::Exception ex)
    {
        LOG( LOG_ERROR_TYPE, tr("Erro %1  na tentativa de criar um  novo dicionario  de simbolos").arg(QString::fromStdString(ex.msg)));
        return false;
    }


    try
    {

        if(!contours_found_LetterLower.isEmpty() && (CheckEqualAllElement(contours_found_LetterLower)==false) && (contours_found_LetterLower.size() > 2))
        {
            Mat contornos_Letter =ExtraiContornos(contours_found_LetterLower).clone();
            Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(contornos_Letter, cv::ml::ROW_SAMPLE, ExtraiLabels());
            svm->train(td);
            svm->save(fnameLetter_lower.toLatin1().data());
        }
    }
    catch (cv::Exception ex)
    {
        LOG( LOG_ERROR_TYPE, tr("Erro %1  na tentativa de criar um  novo dicionario de letras MINUSCULAS").arg(QString::fromStdString(ex.msg)));
        return false;
    }

    try
    {

        if(!contours_found_LetterUpper.isEmpty() && (CheckEqualAllElement(contours_found_LetterUpper) == false) && (contours_found_LetterUpper.size() > 2))
        {
            Mat contornos_Upper = ExtraiContornos(contours_found_LetterUpper).clone();
            Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(contornos_Upper, cv::ml::ROW_SAMPLE, ExtraiLabels());
            svm->train(td);
            svm->save(fnameLetter_upper.toLatin1().data());
        }

    }
    catch (cv::Exception ex)
    {
        LOG( LOG_ERROR_TYPE, tr("Erro %1  na tentativa de criar um  novo dicionario de letras MAIUSCULA").arg(QString::fromStdString(ex.msg)));
        return false;
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool ManegerLibrary::SaveDictionary(const QString &fname)
{
    if(contours_found.isEmpty()) return false;

    if(!SaveObject(contours_found,fname))
    {
        dlg.close();
        return false;
    }

    dlg.close();
}

/// ===========================================================================
///
/// ===========================================================================
bool ManegerLibrary::LoadDictionary()
{
  contours_found = ListObject(QString("./data/hog_library/"));
  qSort(contours_found.begin(),contours_found.end(),CompareLessX2);
}

/// ===========================================================================
///
/// ===========================================================================
void ManegerLibrary::SetItem(const MvOCRPO::DESCRIPTOR contours)
{
   contours_found.push_back(contours);
   qSort(contours_found.begin(),contours_found.end(),CompareLessX2);
}

/// ===========================================================================
///
/// ===========================================================================
void ManegerLibrary::RemoveItem(const int current_index)
{
    contours_found.removeAt(current_index);
    qSort(contours_found.begin(),contours_found.end(),CompareLessX2);
}

/// ===========================================================================
///
/// ===========================================================================
QList<MvOCRPO::DESCRIPTOR> ManegerLibrary::GetLibrary()
{
    return contours_found;
}

/// ===========================================================================
///
/// ===========================================================================
bool ManegerLibrary::GetItem(const int current_index, MvOCRPO::DESCRIPTOR &contours)
{
    if( current_index < 0 || current_index >= contours_found.size() ) return false;

   contours = contours_found.at(current_index);

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
Mat ManegerLibrary::ExtraiContornos(QList<MvOCRPO::DESCRIPTOR> contours_found)
{
       labels.clear();

       if(contours_found.isEmpty()) return Mat();

       qSort(contours_found.begin(),contours_found.end(),CompareLessX2);

       int  valid_labels = 0;

       for( int index = 0; index < contours_found.size(); index++ )
       {
           if( contours_found[index].character.isNull() == false ) valid_labels++;
       }

       cv::Mat trainingMat(valid_labels , contours_found.at(0).mat.cols, CV_32FC1);

       for( int index = 0; index < contours_found.size(); index++ )
       {
           if( contours_found[index].character.isNull() ) continue;

           Mat m = contours_found[index].mat.clone();

           int dest_col = 0;
           int dest_row = labels.size();

           for (int j = 0; j < m.cols; j++)
           {
               trainingMat.at<float>(dest_row, dest_col++) = m.at<float>(0,j);
           }

           labels.push_back(contours_found[index].character.toLatin1());
       }

       return trainingMat;

}

/// ===========================================================================
///
/// ===========================================================================
Mat ManegerLibrary::ExtraiLabels()
{
       if(labels.isEmpty()==true) false;

       int labelsArray[labels.size()];

       for( int index = 0; index < labels.size(); index++ )
       {
           labelsArray[index] = labels[index];
       }

       Mat Matlabels(labels.size(), 1, CV_32S, labelsArray);

       return Matlabels.clone();
}

/// ===========================================================================
///
/// ===========================================================================
void ManegerLibrary::Clear()
{
    contours_found.clear();
    contours_found_Numbers.clear();
    contours_found_Simbol.clear();
    contours_found_LetterUpper.clear();
    contours_found_LetterLower.clear();

}

/// ===========================================================================
///
/// ===========================================================================
void ManegerLibrary::Extracted_Dictionary()
{
    if(contours_found.isEmpty()) return;

       foreach ( MvOCRPO::DESCRIPTOR c, contours_found)
       {
           if(c.character.isNumber())    contours_found_Numbers.push_back(c);
           if(c.character.isPunct())     contours_found_Simbol.push_back(c);
           if(c.character.isLower())     contours_found_LetterLower.push_back(c);
           if(c.character.isUpper())     contours_found_LetterUpper.push_back(c);

       }
}

/// ===========================================================================
///
/// ===========================================================================
bool ManegerLibrary::CheckEqualAllElement(QList<MvOCRPO::DESCRIPTOR> contours_found)
{
    if(contours_found.size() < 2 ) return true ;

    bool all_equal = false;

    std::vector<int> caracter=  caracteres(contours_found).toStdVector();

    if ( std::adjacent_find( caracter.begin(), caracter.end(), std::not_equal_to<>() ) == caracter.end() )
    {
        LOG(LOG_WARNING_TYPE, tr("All elements are equal each other" ));
        all_equal = true;
    }

    return all_equal;
}


