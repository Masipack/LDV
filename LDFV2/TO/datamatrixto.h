#ifndef DATAMATRIXTO_H
#define DATAMATRIXTO_H

#include <QtCore>
#include "TO/abstracttoolto.h"

class DatamatrixTO
{
public:
     AbstractToolTO      BASE;

    //NOT SERIALIZABLE:

    bool hasChanges(const DatamatrixTO& other)
    {
        return ( BASE.EXPECTED_TEXT   != other.BASE.EXPECTED_TEXT  );
    }

    friend QDataStream &operator >>(QDataStream &s, DatamatrixTO& v)
    {
        s >> v.BASE;
        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const DatamatrixTO& v)
    {
        s << v.BASE;
        return s;
    }

    friend QDebug operator <<(QDebug dbg, const DatamatrixTO& v)
    {
        dbg.space() << "\nDatamatrixTO:\n";
        dbg.space() << v.BASE;
        return dbg.space();
    }
};

#endif // DATAMATRIXTO_H
