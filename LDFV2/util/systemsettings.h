#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QtCore>
#include <QIODevice>
#include <QVector>
#include <QSettings>

#include "util/sys_log.h"

#define SSI SystemSettings::instance()

class SystemSettings : public QObject
{
    Q_OBJECT
public:
    SystemSettings(QObject *parent = 0) { Q_UNUSED(parent); m_settingsDB = 0;}
    ~SystemSettings();

    static SystemSettings *instance();

    bool init(const QString& filename);

signals:

public slots:

private:
    QSettings*  m_settingsDB;
    QMutex      mutex;

    /// ===========================================================================
    /// DB WRAPPER
    /// ===========================================================================
    public:
        template <typename T>
        bool readValue(T& destClass, const QString& key)
        {
            QMutexLocker locker(&mutex);
            if( m_settingsDB->contains(key) == false ) return false;
            destClass = m_settingsDB->value(key).value<T>();
            return true;
        }

        template <typename T>
        bool writeValue(const T& destClass, const QString& key )
        {
            QMutexLocker locker(&mutex);
            m_settingsDB->setValue(key, destClass);
            return true;
        }

        bool writeReal(qreal value, const QString& key )
        {
            QMutexLocker locker(&mutex);
            m_settingsDB->setValue(key, QVariant((double)value) );
            return true;
        }

        bool writeRawValue(QByteArray& b, const QString& key )
        {
            QMutexLocker locker(&mutex);

            if( 0 == m_settingsDB ) return false;
            m_settingsDB->setValue(key, b);
            return true;
        }

        bool deleteValue( const QString& key )
        {
            QMutexLocker locker(&mutex);
            if( 0 == m_settingsDB ) return false;
            m_settingsDB->remove(key);
        }

        bool listKeys( QVector< QString >& keyList, const QString& prefix )
        {
            QMutexLocker locker(&mutex);
            if( 0 == m_settingsDB ) return false;
            QStringList list = m_settingsDB->allKeys().filter(prefix);
            keyList = list.toVector();
            return true;
        }

        bool contains( const QString& key )
        {
            QMutexLocker locker(&mutex);

            if( 0 == m_settingsDB ) return false;
            return m_settingsDB->contains(key);
        }

        bool sync() { m_settingsDB->sync(); return true; }

};

template< typename T >
bool GetConfig( T& destVar, const QString& key, const T& defaultValue )
{
    if( SystemSettings::instance()->readValue(destVar, key) == false )
    {
        QString logString = QObject::tr("WARNING: configuration ");
        logString += key;
        logString += QObject::tr(" not found");
        LOG(LOG_WARNING_TYPE, logString );
        destVar = defaultValue;
        return false;
    }
    return true;
}

template< typename T >
bool GetConfig(QSettings& set, T& destVar, const QString& key, const T& defaultValue )
{
    if( set.contains(key) == false )
    {
        QString logString = QObject::tr("WARNING: configuration ");
        logString += key;
        logString += QObject::tr(" not found");
        LOG(LOG_WARNING_TYPE, logString );
        destVar = defaultValue;
        return false;
    }

    destVar = set.value(key).value<T>();
    return true;
}

#endif // SYSTEMSETTINGS_H
