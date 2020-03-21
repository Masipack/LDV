#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QSet>

#define ALM_INIT                    0
#define ALM_CAMERA_NOT_FOUND        1
#define ALM_SERIAL_PORT             2
#define ALM_CAMERA_ERROR            3
#define ALM_ERROR_CALIBRATION       4

class AlarmManager : public QObject
{
    Q_OBJECT
public:
    AlarmManager(QObject *parent = nullptr);
    ~AlarmManager();

    static AlarmManager *instance();

    void SetAlarm(int alarm_bit_num);

    void ClearAlarms() { current_alarms.clear(); }

    void GetAlarmList(QStringList &dest);

    bool HaveAlarm() { return current_alarms.size(); }

signals:
    void NewAlarmList(const QStringList data);

public slots:


private:
    QMap<int, QString>      identification;
    QSet<int>               current_alarms;

};

#endif // ALARMMANAGER_H
