#ifndef ABSTRACTTOOLTO_H
#define ABSTRACTTOOLTO_H

#include <QtCore>

class AbstractToolTO
{
public:
    QString             NAME;
    QRectF              ROI;
    quint8              VISIBLE;
    quint8              FIXED_ANGLE;
    qreal               ANGLE;
    quint32             TYPE;
    QPointF             CONFIG_POS;
    quint32             EXECUTION_ORDER;
    QString             EXPECTED_TEXT;

    //NOT SERIALIZABLE:


    friend QDataStream &operator >>(QDataStream &s, AbstractToolTO& v)
    {
        s >> v.NAME;
        s >> v.ROI;
        s >> v.VISIBLE;
        s >> v.FIXED_ANGLE;
        s >> v.ANGLE;
        s >> v.TYPE;
        s >> v.CONFIG_POS;
        s >> v.EXECUTION_ORDER;
        s >> v.EXPECTED_TEXT;
        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const AbstractToolTO& v)
    {
        s << v.NAME;
        s << v.ROI;
        s << v.VISIBLE;
        s << v.FIXED_ANGLE;
        s << v.ANGLE;
        s << v.TYPE;
        s << v.CONFIG_POS;
        s << v.EXECUTION_ORDER;
        s << v.EXPECTED_TEXT;
        return s;
    }

    friend QDebug operator <<(QDebug dbg, const AbstractToolTO &v)
    {
        dbg.space() << v.NAME;
        dbg.space() << v.ROI;
        dbg.space() << v.VISIBLE;
        dbg.space() << v.FIXED_ANGLE;
        dbg.space() << v.ANGLE;
        dbg.space() << v.TYPE;
        dbg.space() << v.CONFIG_POS;
        dbg.space() << v.EXECUTION_ORDER;
        dbg.space() << v.EXPECTED_TEXT;
        return dbg;
    }
};

#endif // ABSTRACTTOOLTO_H
