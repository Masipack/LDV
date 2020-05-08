#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QtCore>
#include <QObject>
#include "global_defines.h"
#include "TO/databaseto.h"


static QStringList GetFileList(const QString& basePath, const QString& suffix, bool bRemoveSuffix=true)
{
    QDir dir(basePath);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    QStringList filters;
    filters << QString("*.%1").arg(suffix);
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    QStringList existingFiles;

    foreach (QFileInfo f, list)
    {
        QString s = f.fileName();
        if(bRemoveSuffix) s.chop(4);
        existingFiles.push_back( s );
    }

    return existingFiles;
}

template <class T>
bool WriteTO( T& src, const QString& FileName, QString& err_result, bool overwrite = false )
{
    if( overwrite == false && QFile::exists( QString("./data/formats/%1").arg(FileName) ) )
    {
        err_result = QObject::tr("Formato existente");
        return false;
    }

    QFile f(QString("./data/formats/%1").arg(FileName) );
    f.open(QIODevice::WriteOnly);
    QDataStream s(&f);
    s << src;

    if( s.status() != QDataStream::Ok )
    {
        err_result = QObject::tr("ERRO GRAVANDO FORMATO");
        return false;
    }

    err_result.clear();
    return true;
}

template <class T>
bool WriteGenericTO( T& src, const QString& Path_File, QString& err_result, bool overwrite = false )
{
    if( overwrite == false && QFile::exists(Path_File))
    {
        err_result = QObject::tr("Formato existente");
        return false;
    }

    QFile f(QString("%1").arg(Path_File));
    f.open(QIODevice::WriteOnly);
    QDataStream s(&f);
    s << src;

    if( s.status() != QDataStream::Ok )
    {
        err_result = QObject::tr("Erro gravando formato");
        return false;
    }

    err_result.clear();
    return true;
}

template <class T>
bool ReadTO( T& src, const QString& FileName, QString& err_result )
{
    if( QFile::exists( QString("./data/formats/%1").arg(FileName) ) == false )
    {
        err_result = QObject::tr("Formato não encontrado");
        return false;
    }

    QFile f(QString("./data/formats/%1").arg(FileName) );
    f.open(QIODevice::ReadOnly);
    QDataStream s(&f);
    s >> src;


    if( s.status() != QDataStream::Ok )
    {
        err_result = QObject::tr("Erro lendo formato");
        return false;
    }

    err_result.clear();
    return true;
}

template <class T>
bool ReadGenericTO( T& src, const QString& Path_File, QString& err_result )
{
    if( QFile::exists( QString(Path_File)) == false )
    {
        err_result = QObject::tr("Formato não encontrado");
        return false;
    }

    QFile f(QString("%1").arg(Path_File));
    f.open(QIODevice::ReadOnly);
    QDataStream s(&f);
    s >> src;


    if( s.status() != QDataStream::Ok )
    {
        err_result = QObject::tr("Erro lendo formato");
        return false;
    }

    err_result.clear();
    return true;
}

static quint32 ReadTO_TYPE( const QString& FileName, QString& err_result )
{
    if( QFile::exists( QString("./data/formats/%1").arg(FileName) ) == false )
    {
        err_result = QObject::tr("Formato não encontrado");
        return false;
    }

    quint32 TYPE = 0xFF;

    QFile f(QString("./data/formats/%1").arg(FileName) );
    f.open(QIODevice::ReadOnly);
    QDataStream s(&f);
    s >> TYPE;
    if( s.status() != QDataStream::Ok )
    {
        err_result = QObject::tr("Erro lendo formato");
        return false;
    }

    err_result.clear();
    return TYPE;
}

static QString file_size_human(double num)
{
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
     {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num,'f',2)+" "+unit;
}

static void ConvertDATABASETO_To_DATABASE(DATABASE& database, DATABASETO databaseto){

    database.name     = databaseto.name;
    database.password = databaseto.password;
    database.user     = databaseto.user;
    database.url      = databaseto.url;
    database.table    = databaseto.table;
    database.port     = databaseto.port;

}
#endif // FILEUTIL_H
