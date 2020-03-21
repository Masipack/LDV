#ifndef MULTICAMTO_H
#define MULTICAMTO_H

#include "TO/productto.h"

class MultiCamTO
{
public:
    QString                 NAME;
    QVector<ProductTO>      TO;

    //NOT SERIALIZABLE:

    friend QDataStream &operator >>(QDataStream &s, MultiCamTO& v)
    {
        s >> v.NAME;
        s >> v.TO;
        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const MultiCamTO& v)
    {
        s << v.NAME;
        s << v.TO;
        return s;
    }

    friend QDebug operator <<(QDebug dbg, const MultiCamTO& v)
    {
        dbg.space() << "\nMultiCamTO:\n";
        dbg.space() << v.NAME;
        dbg.space() << v.TO;
        return dbg.space();
    }
};

#endif // MULTICAMTO_H
