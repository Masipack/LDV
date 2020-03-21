#ifndef DATABASETO_H
#define DATABASETO_H

#include <QtCore>

class DATABASETO
{
public:

    QString  url;
    QString  name;
    QString  user;
    QString  password;
    QString  port;
    QString  table;
    QString  parameter_search;


    //NOT SERIALIZABLE:

    DATABASETO()
    {;}

    friend QDataStream &operator >>(QDataStream &s, DATABASETO& v)
    {
        s >> v.url;
        s >> v.name;
        s >> v.user;
        s >> v.password;
        s >> v.port;
        s >> v.table;
        s >> v.parameter_search;

        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const DATABASETO& v)
    {

        s << v.url;
        s << v.name;
        s << v.user;
        s << v.password;
        s << v.port;
        s << v.table;
        s << v.parameter_search;

        return s;
    }

    friend QDebug operator <<(QDebug dbg, const DATABASETO &v)
    {
          dbg.space() << "\nDB LOAD:\n";
          dbg.space() << v.url;
          dbg.space() << v.name;
          dbg.space() << v.user;
          dbg.space() << v.password;
          dbg.space() << v.port;
          dbg.space() << v.table;
          dbg.space() << v.parameter_search;

        return dbg.space();
    }
};
#endif // DATABASETO_H
