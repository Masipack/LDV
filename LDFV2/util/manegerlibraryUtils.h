#ifndef MANEGERLIBRARYUTILS_H
#define MANEGERLIBRARYUTILS_H

#include "mv/PO/mvocrpo.h"
#include "TO/libraryto.h"
#include "util/fileutil.h"
#include "util/sys_log.h"
#include "QtCore"
#include "qdebug.h"

/// ===========================================================================
///
/// ===========================================================================
static bool CompareLessX( MvOCRPO::DESCRIPTOR c1, MvOCRPO::DESCRIPTOR  c2)
{
    if(c1.character.toLatin1() > c2.character.toLatin1()) return  c1.character.toLatin1() > c2.character.toLatin1();
    return c2.character.toLatin1() < c2.character.toLatin1();

}

/// ===========================================================================
///
/// ===========================================================================
static bool CompareLessX2(MvOCRPO::DESCRIPTOR c1, MvOCRPO::DESCRIPTOR  c2)
{
    if(c1.character.toLatin1() < c2.character.toLatin1()) return  c1.character.toLatin1() < c2.character.toLatin1();
    return c2.character.toLatin1() > c2.character.toLatin1();

}

/// ===========================================================================
///
/// ===========================================================================
static QList<MvOCRPO::DESCRIPTOR>  ListObject(const QString& Filename)
    {

       QList<MvOCRPO::DESCRIPTOR> contours_found;

       LibratyTO TO;
       QString error;

       if(!ReadTO(TO,QString("hog.ldata"),Filename,error))
       {
          LOG(LOG_ERROR_TYPE,error);
          return   QList<MvOCRPO::DESCRIPTOR>();
       }

       for (int i = 0; i < TO.imagem.length(); ++i)
       {
            MvOCRPO::DESCRIPTOR c;
            c.descriptors = TO.descriptors[i];
            c.character   = TO.character[i];
            c.image       = TO.imagem[i].copy();
            c.mat         = Mat(1, TO.descriptors[i].length(), CV_32FC1);

            for( int n = 0; n < c.descriptors.length(); n++ )
            {
               c.mat.at<float>(0, n) =  c.descriptors.at(n);
            }

            contours_found.push_back(c);

        }

       return contours_found;
}


/// ===========================================================================
///
/// ===========================================================================
static bool SaveObject(QList<MvOCRPO::DESCRIPTOR> contours_found, const QString& Filename)
{
        LibratyTO TO;

        if(contours_found.isEmpty()) return false;

        foreach (MvOCRPO::DESCRIPTOR  coutour, contours_found)
        {
            TO.imagem.push_back( coutour.image.copy() );
            TO.character.push_back( coutour.character );
            TO.descriptors.push_back(coutour.descriptors);

        }

        QString error;

        if(!WriteTO(TO,QString("hog.ldata"),Filename,error))
        {
          LOG( LOG_ERROR_TYPE,error);
          return false;
        }

        return true;
}

#endif // MANEGERLIBRARYUTILS_H
