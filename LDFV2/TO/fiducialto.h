#ifndef FIDUCIALTO_H
#define FIDUCIALTO_H

#include <QtCore>
#include "TO/abstracttoolto.h"

class FiducialTO
{
public:
    AbstractToolTO  BASE;
    QByteArray      IMG_TEMPLATE;
    qreal           MIN_CORRELATION;
    QSize           IMG_TEMPLATE_SZ;

    //NOT SERIALIZABLE:
    bool hasChanges(const FiducialTO& other)
    {
       return ( MIN_CORRELATION   != other.MIN_CORRELATION  );
    }

    friend QDataStream &operator >>(QDataStream &s, FiducialTO& v)
    {
        s >> v.BASE;
        s >> v.IMG_TEMPLATE;
        s >> v.MIN_CORRELATION;
        s >> v.IMG_TEMPLATE_SZ;
        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const FiducialTO& v)
    {
        s << v.BASE;
        s << v.IMG_TEMPLATE;
        s << v.MIN_CORRELATION;
        s << v.IMG_TEMPLATE_SZ;
        return s;
    }

    friend QDebug operator <<(QDebug dbg, const FiducialTO &v)
    {
        dbg.space() << "\nFiducialTO:\n";
        dbg.space() << v.BASE;
        dbg.space() << v.IMG_TEMPLATE.size();
        dbg.space() << v.MIN_CORRELATION;
        dbg.space() << v.IMG_TEMPLATE_SZ;
        return dbg.space();
    }
};

#endif // FIDUCIALTO_H
