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
    QString         TABLE_ATTRIBUTE;
    int             BLACK_WHITE;

    //NOT SERIALIZABLE:



    bool hasChanges(const OcrTO& other)
    {
         return (( BASE.EXPECTED_TEXT   != other.BASE.EXPECTED_TEXT  ) ||
        (    WHITE_FILTER         != other.WHITE_FILTER        ) || ( BLACK_WHITE!= other.BLACK_WHITE ) ) ;

    }

    friend QDataStream &operator >>(QDataStream &s, OcrTO& v)
    {
        s >> v.BASE;
        s >> v.WHITE_FILTER;
        s >> v.TABLE_ATTRIBUTE;
        s >> v.BLACK_WHITE;
        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const OcrTO& v)
    {
        s << v.BASE;
        s << v.WHITE_FILTER;
        s << v.TABLE_ATTRIBUTE;
        s << v.BLACK_WHITE;
        return s;
    }

    friend QDebug operator <<(QDebug dbg,const OcrTO& v)
    {
        dbg.space() << "\nOcrTO:\n";
        dbg.space() << v.BASE;
        dbg.space() << v.WHITE_FILTER;
        dbg.space() << v.TABLE_ATTRIBUTE;
        dbg.space() << v.BLACK_WHITE;

        return dbg.space();
    }
};

#endif // OCRTO_H
