#ifndef ABSTRACTDATABASE_H
#define ABSTRACTDATABASE_H

#include <QtCore>
#include <QObject>
#include "global_defines.h"
#include <QtSql>

class MvAbstractDataBase
{
    Q_OBJECT
public:
    explicit MvAbstractDataBase(DATABASE   database);
    virtual ~MvAbstractDataBase();

    // Funcoes virtuais puras
    virtual bool Int()                 = 0;
    virtual void Close()               = 0;

private:
    DATABASE   database;

};
#endif // ABSTRACTDATABASE_H
