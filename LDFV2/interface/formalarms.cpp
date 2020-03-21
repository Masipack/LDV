#include "formalarms.h"
#include "ui_formalarms.h"

#include "interface/windowmanager.h"
#include "util/alarmmanager.h"
#include "util/sys_log.h"

/// ===========================================================================
///
/// ===========================================================================
FormAlarms::FormAlarms(QWidget *parent) : QWidget(parent),
    ui(new Ui::FormAlarms)
{
    ui->setupUi(this);
    connect(AlarmManager::instance(), SIGNAL(NewAlarmList(QStringList)), this, SLOT(NewAlarmList(QStringList)) );

    month_names << tr("JANEIRO")   << tr("FEVEREIRO") << tr("MARÇO")
                << tr("ABRIL")     << tr("MAIO")      << tr("JUNHO")
                << tr("JULHO")     << tr("AGOSTO")    << tr("SETEMBRO")
                << tr("OUTUBRO")  << tr("NOVEMBRO")  << tr("DEZEMBRO");

}

/// ===========================================================================
///
/// ===========================================================================
FormAlarms::~FormAlarms()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::NewAlarmList(QStringList data)
{
    ui->lst_alarms->clear();
    ui->lst_alarms->addItems(data);
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)
    WindowManager::instance()->SetInfoTop( tr("Alarmes") );

    ui->btn_alarms->setChecked(true);

    current_month = QDate::currentDate().month()-1;
    current_year  = 0;
    current_day   = QDate::currentDate().day();

    ui->calendarWidget->setCurrentPage(QDate::currentDate().year() + current_year, current_month+1);

    ui->le_date->setText( month_names[current_month] + " " +
                          QString::number(QDate::currentDate().year() + current_year) );

    UpdateLogList();

}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::on_btn_alarms_toggled(bool checked)
{
    if( checked ) ui->stackedWidget->setCurrentIndex(0);
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::on_btn_logs_toggled(bool checked)
{
    if( checked ) ui->stackedWidget->setCurrentIndex(1);
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::on_btn_return_clicked()
{
    WindowManager::instance()->ShowLastScreen();
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::on_btn_reset_alarms_clicked()
{
    P11(tr("Reset de alarmes"), false);
    AlarmManager::instance()->ClearAlarms();
    ui->lst_alarms->clear();
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::on_btn_minus_month_clicked()
{
    if( current_month == 0 )
    {
        current_year--;
        current_month = 11;
    }
    else
    {
        current_month--;
    }

    ui->le_date->setText( month_names[current_month] + " " +
                          QString::number(QDate::currentDate().year() + current_year ) );

    ui->calendarWidget->setCurrentPage(QDate::currentDate().year() + current_year, current_month+1);
    UpdateLogList();
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::on_btn_plus_month_clicked()
{
    if( current_month == 11 )
    {
        current_year++;
        current_month = 0;
    }
    else
    {
        current_month++;
    }

    ui->le_date->setText( month_names[current_month] + " " +
                          QString::number(QDate::currentDate().year() + current_year) );

    ui->calendarWidget->setCurrentPage(QDate::currentDate().year() + current_year, current_month+1);

    UpdateLogList();
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::on_calendarWidget_clicked(const QDate &date)
{
    current_day   = date.day();
    UpdateLogList();
}

/// ===========================================================================
///
/// ===========================================================================
void FormAlarms::UpdateLogList()
{
    ui->lst_logs->clear();

    QDate d(QDate::currentDate().year() + current_year, current_month+1, current_day);
    QString filename = "./log/" + d.toString("yyyy-MM-dd") + ".log";

    if( QFile::exists( filename ) )
    {
        ui->lst_logs->setUpdatesEnabled(false);
        QFile f(filename);
        f.open(QIODevice::ReadOnly);
        f.seek(0);

        while( !f.atEnd() )
        {
            QByteArray line = f.readLine();
            if(line[12] == '|' && line[14] == '|' )
            {
                ui->lst_logs->insertItem(0, line );
            }
        }
        f.close();
        ui->lst_logs->setUpdatesEnabled(true);
    }
}
