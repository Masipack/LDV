#ifndef MSQLSINGLETON_H
#define MSQLSINGLETON_H

#include <QObject>
#include "util/abstractdatabase.h"
#include <QtSql>
#include "util/factorymssql.h"


class MSQLSingleton
{
public:
    inline MSQLSingleton(){ mvMSSQl= nullptr;}
    ~MSQLSingleton();

    static MSQLSingleton *instance();

    bool Init(const DATABASE& database, const QString drive_sql_name);

    FactoryMSSQL*  GetFactorySQL() { return mvMSSQl;}

private:

     FactoryMSSQL*  mvMSSQl;

};

#endif // MSQLSINGLETON_H
