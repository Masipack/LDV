#include "systemsettings.h"

Q_GLOBAL_STATIC(SystemSettings, systemSettings)

/// ===========================================================================
///
/// ===========================================================================
SystemSettings *SystemSettings::instance()
{
    return systemSettings();
}


/// ===========================================================================
///
/// ===========================================================================
SystemSettings::~SystemSettings()
{

}

/// ===========================================================================
///
/// ===========================================================================
bool SystemSettings::init(const QString& filename)
{
    if( m_settingsDB ) delete m_settingsDB;

    try {
         m_settingsDB = new QSettings(filename, QSettings::IniFormat);

    } catch (QException &ex) {

        LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL:%1").arg(QString::fromLocal8Bit(ex.what())));
    }

    return true;
}
