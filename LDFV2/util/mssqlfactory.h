#ifndef MSSQLFACTORY_H
#define MSSQLFACTORY_H

#include <QObject>
#include  "abstractdatabase.h"
class MSSQLFactory: MvAbstractDataBase
{
public:
    MSSQLFactory(const DATABASE& database, const QString drive_sql_nam);


    bool Init();
    void Close();
    bool IsOpen();

     QMap<QString,QString>  GetByID(const QString& ID)const;
};

#endif // MSSQLFACTORY_H
