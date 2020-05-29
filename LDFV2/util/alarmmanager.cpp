#include "alarmmanager.h"

#include "util/systemsettings.h"

Q_GLOBAL_STATIC(AlarmManager, alarmManager)

/// ===========================================================================
///
/// ===========================================================================
AlarmManager::AlarmManager(QObject *parent)
{
    Q_UNUSED(parent);

    Init();
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

/// ===========================================================================
///
/// ===========================================================================
void AlarmManager::Init()
{
    QString s_lang, s_locale;

    GetConfig(s_lang    , "SYSTEM/LANGUAGE"     , QString("pt-br") );


    if(s_lang=="bs")
    {
        QSettings       alarm_names("./data/alarms_bs.dat", QSettings::IniFormat);
        QStringList     names = alarm_names.childKeys();

        for( QString n : names )
        {
            identification.insert(n.toInt(),alarm_names.value(n).toString());
        }
        return ;

    }

    if(s_lang=="en-US")
    {
        QSettings       alarm_names("./data/alarms_enus.dat", QSettings::IniFormat);
        QStringList     names = alarm_names.childKeys();

        for( QString n : names )
        {
            identification.insert(n.toInt(),alarm_names.value(n).toString());
        }
        return;
    }

    QSettings       alarm_names("./data/alarms_ptbr.dat", QSettings::IniFormat);
    QStringList     names = alarm_names.childKeys();

    for( QString n : names )
    {
        identification.insert(n.toInt(),alarm_names.value(n).toString());
    }
    return ;
}
