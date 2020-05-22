#ifndef DVRLOG_H
#define DVRLOG_H

#include <QtCore>
#include <QSqlQuery>
#include "util/dialogpart11.h"

#define LOG_INFO_TYPE       ("I")
#define LOG_WARNING_TYPE    ("W")
#define LOG_ERROR_TYPE      ("E")
#define LOG_FATAL_TYPE      ("F")

/// ===========================================================================
///
/// ===========================================================================
static inline void LOG(const QString& type, const QString& msg)
{
    QString final = "|";
    final += type;
    final += "|";
    final += qApp->property("CURRENT_USER").toString();
    final += "|";
    final += msg;
    qDebug() << final.toUtf8().data(); // utf-8 for brazilian characters
}

/// ===========================================================================
///
/// ===========================================================================
static bool P11(const QString& desc, bool bDialog = false, QString comment = "")
{
    if( qApp->property("USE_PART11").toBool() == false ) return true;

    QString user = qApp->property("CURRENT_USER").toString();

    if( bDialog )
    {
        DialogPart11 d;
        if( d.exec() == QDialog::Rejected  ) return false;
        QString s = d.GetComment();
        comment = s;
        user = d.GetUser();
    }


    QSqlQuery query(QSqlDatabase::database("PART11"));
    query.prepare("INSERT INTO Audit (RecordID, TimeStamp, Lote, UserID, ObjectID, Description, Comment, Checksum) VALUES (:RecordID, :TimeStamp, :Lote, :UserID, :ObjectID, :Description, :Comment, :Checksum)");
    query.bindValue(":TimeStamp"        , QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") );
    query.bindValue(":Lote"             , qApp->property("BATCH").toString() );
    query.bindValue(":UserID"           , user);
    query.bindValue(":ObjectID"         , "LDF System");
    query.bindValue(":Description"      , desc);
    query.bindValue(":Comment"          , comment);

    bool result = query.exec();

    return result;

    return true;
}

#endif // SYS_LOG_H
