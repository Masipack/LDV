#ifndef BARCODETO_H
#define BARCODETO_H

#include <QtCore>
#include "TO/abstracttoolto.h"

class BarcodeTO
{
public:
    // NO ITEMS... (yet)
    AbstractToolTO      BASE;

    //NOT SERIALIZABLE:

    bool hasChanges(const BarcodeTO& other)
    {
        return ( BASE.EXPECTED_TEXT   != other.BASE.EXPECTED_TEXT  ) ;
    }

    friend QDataStream &operator >>(QDataStream &s, BarcodeTO& v)
    {
        s >> v.BASE;
        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const BarcodeTO& v)
    {
        s << v.BASE;
        return s;
    }

    friend QDebug operator <<(QDebug dbg, const BarcodeTO& v)
    {
        dbg.space() << "\nBarcodeTO:\n";
        dbg.space() << v.BASE;
        return dbg.space();
    }
};

#endif // BARCODETO_H
