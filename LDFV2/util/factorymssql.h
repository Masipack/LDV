#ifndef FACTORYMSSQL_H
#define FACTORYMSSQL_H

#include <QObject>
#include "abstractdatabase.h"
#include "QSqlDatabase"
#include "QSqlQueryModel"
#include "QSqlQuery"
#include "global_defines.h"


class FactoryMSSQL
{
public:
    FactoryMSSQL();
    ~FactoryMSSQL();
    bool Init(const DATABASE database, const QString drive_sql_name);
    void Close();
    bool IsOpen();

    QMap<QString,QString>  GetByID(const QString& ID)const;
    QMap<QString,QString>  GetAll()const;

    void setAttribute(const QString attribute){ this->attribute = attribute; }

    void     SetDataBase(DATABASE database){this->database= database;}
    DATABASE GetDataBase() {return database;}


protected:
    bool   connectToServer(const QString driver, DATABASE database);


private:
    QSqlDatabase  db_local;
    QString       attribute;
    DATABASE      database;

    static Result externConnectFunction(QSqlDatabase &db);
};

#endif // FACTORYMSSQL_H
