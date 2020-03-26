#include "factorymssql.h"
#include "QSqlRecord"
#include <QtConcurrent/QtConcurrentRun>



/// ===========================================================================
///
/// ===========================================================================
FactoryMSSQL::FactoryMSSQL()
{

}

/// ===========================================================================
///
/// ===========================================================================
FactoryMSSQL::~FactoryMSSQL()
{
    db_local.close();
}

/// ===========================================================================
///
/// ===========================================================================
bool FactoryMSSQL::Init(const DATABASE database, const QString drive_sql_name)
{
  this-> database = database;
  return connectToServer(drive_sql_name,database);
}

/// ===========================================================================
///
/// ===========================================================================
void FactoryMSSQL::Close()
{
   db_local.close();
}

/// ===========================================================================
///
/// ===========================================================================
bool FactoryMSSQL::IsOpen()
{
   if(!db_local.isValid()) return false;
   return db_local.isOpen();
}

/// ===========================================================================
///
/// ===========================================================================
QMap<QString, QString> FactoryMSSQL::GetByID(const QString &ID) const
{
    QMap<QString,QString> result;

    if(!db_local.isOpen()|| db_local.isValid()==false){ return  QMap<QString,QString>() ;}

    QString str_query = QString("select * from %1 where %2= '%3'").arg(database.table).arg(attribute).arg(ID);

    Debug(str_query)
    QSqlQueryModel *model = new QSqlQueryModel;

    QSqlQuery query_CON(str_query, db_local);

    QSqlQuery query(QString(), db_local);
    model->setQuery(query_CON);
    query.exec(str_query);

    int columncount= model->columnCount();

    if(query.next())
    {
        for (int nk = 0; nk < columncount; ++nk)
        {
            result.insert(query.record().fieldName(nk),query.value(nk).toString());
        }
    }

    return result;
}

/// ===========================================================================
///
/// ===========================================================================
QMap<QString, QString> FactoryMSSQL::GetAll() const
{
    QMap<QString,QString> result;

    if( db_local.isOpen()|| db_local.isValid()==false){ QMap<QString,QString>();}

    QString str_query = QString("select * from %1").arg(database.table);

    QSqlQueryModel *model = new QSqlQueryModel;

    QSqlQuery query_CON(str_query, db_local);

    QSqlQuery query(QString(), db_local);
    model->setQuery(query_CON);
    query.exec(str_query);

    int columncount= model->columnCount();

    if(query.next())
    {
        for (int nk = 0; nk < columncount; ++nk)
        {
            result.insert(query.record().fieldName(nk),query.value(nk).toString());
        }
    }

    return result;
}

/// ===========================================================================
///
/// ===========================================================================
bool FactoryMSSQL::connectToServer(const QString driver, DATABASE database)
{
   db_local = QSqlDatabase::addDatabase("QODBC", database.name);

   db_local.setDatabaseName(QStringLiteral("DRIVER={%1};"
                                            "SERVER=%2;"
                                            "PORT=%3;"
                                            "DATABASE=%4;"
                                            "UID=%5;"
                                            "PWD=%6;"
                                            ).arg(driver).arg(database.url).arg(database.port).arg(database.name)
                                           .arg(database.user).arg(database.password));

   QFuture<Result> future = QtConcurrent::run(externConnectFunction,this->db_local);

   return future.result().value;
}

/// ===========================================================================
///
/// ===========================================================================
Result  FactoryMSSQL::externConnectFunction(QSqlDatabase& db)
{
    Result result;
    result.value = db.open();

    return result;
}
