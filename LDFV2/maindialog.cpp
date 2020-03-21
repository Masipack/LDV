#include "maindialog.h"
#include "ui_maindialog.h"

#include <QtCore>
#include "interface/windowmanager.h"
#include "util/alarmmanager.h"

/// ===========================================================================
///
/// ===========================================================================
MainDialog::MainDialog(QWidget *parent) : QDialog(parent),  ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    connect(&timer_date, SIGNAL(timeout()), this, SLOT(update_time()) );
    timer_date.start(500);

    WindowManager::instance()->SetInfoLabels( ui->lbl_top, 0 );
    WindowManager::instance()->SetUserLabels(ui->lbl_user, ui->lbl_level);

    ui->btn_alarm->setProperty("ALARM", 0);
}

/// ===========================================================================
///
/// ===========================================================================
MainDialog::~MainDialog()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
QWidget* MainDialog::GetMainWidget()
{
    return ui->main_widget;
}

/// ===========================================================================
///
/// ===========================================================================
void MainDialog::update_time()
{
    ui->lbl_time->setText( QTime::currentTime().toString("hh:mm:ss"));
    ui->lbl_date->setText( QDateTime::currentDateTime().toString("yyyy-MM-dd"));

    if( AlarmManager::instance()->HaveAlarm() )
    {
        if( ui->btn_alarm->property("ALARM").toInt() == 0 )
        {
            ui->btn_alarm->setIcon(QIcon(":/images/square_alarm.png"));
        }

        if( ui->frame_2->styleSheet() == "background-color: rgb(200, 0, 0);" )
            ui->frame_2->setStyleSheet("background-color: rgb(120, 0, 0);");
        else
            ui->frame_2->setStyleSheet("background-color: rgb(200, 0, 0);");

        ui->btn_alarm->setProperty("ALARM", 1);
    }
    else
    {
        if( ui->btn_alarm->property("ALARM").toInt() == 1 )
        {
            ui->btn_alarm->setIcon(QIcon(":/images/square_logo.png"));
            ui->frame_2->setStyleSheet("background-color: rgb(47, 80, 111);");
            ui->btn_alarm->setProperty("ALARM", 0);
        }
    }
}

/// ===========================================================================
///
/// ===========================================================================
void MainDialog::on_btn_login_clicked()
{
    WindowManager::instance()->ShowScreen("Login");
}

/// ===========================================================================
///
/// ===========================================================================
void MainDialog::on_btn_alarm_clicked()
{
    WindowManager::instance()->ShowScreen("Alarms");
}
