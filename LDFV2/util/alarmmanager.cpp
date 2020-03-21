#include "alarmmanager.h"

#include "util/systemsettings.h"

Q_GLOBAL_STATIC(AlarmManager, alarmManager)

/// ===========================================================================
///
/// ===========================================================================
AlarmManager::AlarmManager(QObject *parent)
{
    Q_UNUSED(parent);
    QSettings       alarm_names("./data/alarms_ptbr.dat", QSettings::IniFormat);
    QStringList     names = alarm_names.childKeys();

    for( QString n : names )
    {
        identification.insert(n.toInt(),alarm_names.value(n).toString());
    }
}

/// ===========================================================================
///
/// ===========================================================================
AlarmManager *AlarmManager::instance()
{
    return alarmManager();
}


/// ===========================================================================
///
/// ===========================================================================
AlarmManager::~AlarmManager()
{

}

/// ===========================================================================
///
/// ===========================================================================
void AlarmManager::SetAlarm(int alarm_num)
{
    current_alarms.insert(alarm_num);
    QStringList list;
    for( int c : current_alarms )
    {
        list << identification[c];
    }
    emit( NewAlarmList(list) );
}

/// ===========================================================================
///
/// ===========================================================================
void AlarmManager::GetAlarmList(QStringList& dest)
{
}
