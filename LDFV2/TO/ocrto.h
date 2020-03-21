#ifndef OCRTO_H
#define OCRTO_H

#include <QtCore>
#include "TO/abstracttoolto.h"
#include "global_defines.h"

class OcrTO
{
public:
    AbstractToolTO  BASE;
    quint32         WHITE_FILTER;

    //NOT SERIALIZABLE:


    bool hasChanges(const OcrTO& other)
    {
         return (( BASE.EXPECTED_TEXT   != other.BASE.EXPECTED_TEXT  ) ||
        (    WHITE_FILTER         != other.WHITE_FILTER        )) ;

    }
    friend QDataStream &operator >>(QDataStream &s, OcrTO& v)
    {
        s >> v.BASE;
        s >> v.WHITE_FILTER;
        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const OcrTO& v)
    {
        s << v.BASE;
        s << v.WHITE_FILTER;
        return s;
    }

    friend QDebug operator <<(QDebug dbg,const OcrTO& v)
    {
        dbg.space() << "\nOcrTO:\n";
        dbg.space() << v.BASE;
        dbg.space() << v.WHITE_FILTER;
        return dbg.space();
    }
};

#endif // OCRTO_H
