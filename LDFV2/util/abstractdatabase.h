#ifndef ABSTRACTDATABASE_H
#define ABSTRACTDATABASE_H

#include <QtCore>
#include <QObject>
#include "global_defines.h"


class  MvAbstractDataBase
{

public:
      // Funcoes virtuais puras
    virtual bool Init(DATABASE& database, const QString drive_sql_name)= 0;
    virtual void Close()                  = 0;
    virtual bool IsOpen()                 = 0;


};
#endif // ABSTRACTDATABASE_H
