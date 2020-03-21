#include "msqlsingleton.h"
#include "QSqlQueryModel"
#include "QSqlQuery"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(MSQLSingleton, msqlSingleton)

/// ===========================================================================
///
/// ===========================================================================
MSQLSingleton *MSQLSingleton::instance()
{
   return   msqlSingleton();
}

/// ===========================================================================
///
/// ===========================================================================
MSQLSingleton::~MSQLSingleton()
{
   delete mvMSSQl;
}

/// ===========================================================================
///
/// ===========================================================================
bool MSQLSingleton::Init(const DATABASE &database, const QString drive_sql_name)
{
      mvMSSQl = new FactoryMSSQL();
      return mvMSSQl->Init(database,drive_sql_name);
}
