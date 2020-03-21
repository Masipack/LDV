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
    m_settingsDB = new QSettings(filename, QSettings::IniFormat);
    return true;
}
