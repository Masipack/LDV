#include "formconfig.h"
#include "ui_form_configH.h"
#include "ui_form_configV.h"

#include "interface/windowmanager.h"
#include "util/sys_log.h"
#include "global_defines.h"

#ifdef _WIN32
#include <windows.h>
#endif

/// ===========================================================================
///
/// ===========================================================================
FormConfig::FormConfig(QWidget *parent) :  QWidget(parent)
{
    ui = 0;
#ifdef _DISPLAY_HORIZONTAL_
    ui = new Ui::FormConfigH;
#else
    ui = new Ui::FormConfigV;
#endif
    ui->setupUi(this);

     ui->btn_database->setVisible( qApp->property("USE_DATABASE").toBool() );
     ui->btn_language->setVisible( qApp->property("USE_LANGUAGE").toBool() );
     ui->btn_restart->setVisible( qApp->property("USE_RESTART").toBool() );
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    if(WindowManager::instance()->GetCurrentUserLevel() == -1 )
    {
        WindowManager::instance()->ShowScreen("MainMenu");
        return;
    }

    WindowManager::instance()->SetInfoTop(tr("Configurações"));

    ui->lbl_hour->setText(QString("%1").arg(QTime::currentTime().hour(),2, 10, QChar('0')));
    ui->lbl_min->setText(QString("%1").arg(QTime::currentTime().minute(),2, 10, QChar('0')));
    ui->lbl_y->setText(QString("%1").arg(QDate::currentDate().year(), 4, 10, QChar('0')));
    ui->lbl_m->setText(QString("%1").arg(QDate::currentDate().month(), 2, 10, QChar('0')));
    ui->lbl_d->setText(QString("%1").arg(QDate::currentDate().day(), 2, 10, QChar('0')));

    ui->btn_apply_date->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_apply_time->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_database->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_maneger_output->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_menu->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_minus_d->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_minus_hour->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_minus_m->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_minus_min->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
    ui->btn_minus_y->setEnabled(WindowManager::instance()->GetCurrentUserLevel() == -1 ?false:true);
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_menu_clicked()
{
    WindowManager::instance()->ShowScreen( "MainMenu" );
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_apply_time_clicked()
{
#ifndef _WIN32
    QProcess p;
    int vh = ui->lbl_hour->text().toInt();
    int vm = ui->lbl_min->text().toInt();

    //QString cmd = "sudo date +%T -s " + QString("\"%1:%2:00\"").arg(vh, 2, 10, QChar('0')).arg(vm, 2, 10, QChar('0'));
    QString dt = "\"" +
            QDate::currentDate().toString("yyyyMMdd") +
            QString(" %1:%2").arg(vh, 2, 10, QChar('0')).arg(vm, 2, 10, QChar('0')) +
            "\"";

    QString atual = QTime::currentTime().toString("hh:mm");
    QString nova  = QString(" %1:%2").arg(vh, 2, 10, QChar('0')).arg(vm, 2, 10, QChar('0'));
    if( P11(tr("Horário de sistema alterado de ") + atual + tr(" para ") + nova, true ) == false ) return;

    QString cmd;

    cmd = "sudo date --set " + dt;
    p.execute(cmd);
    p.waitForFinished(1000);

    cmd = "sudo hwclock --set --date " + dt;
    p.execute(cmd);
    p.waitForFinished(1000);

    // nano /etc/sudoers
    // ms ALL=(ALL) NOPASSWD: /bin/date
    // sudo date +%T -s "01:07:00"
#else

    int vh = ui->lbl_hour->text().toInt();
    int vm = ui->lbl_min->text().toInt();

    SYSTEMTIME st;
    GetSystemTime(&st);
    st.wHour     =   vh;
    st.wMinute   =   vm;
    st.wSecond   =   0;

    SetSystemTime(&st);
#endif


}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_apply_date_clicked()
{
    QProcess p;

    int vy = ui->lbl_y->text().toInt();
    int vm = ui->lbl_m->text().toInt();
    int vd = ui->lbl_d->text().toInt();

    //QString cmd = "sudo date +%Y%m%d -s " + QString("\"%1%2%3\"").arg(vy, 4, 10, QChar('0')).arg(vm, 2, 10, QChar('0')).arg(vd, 2, 10, QChar('0'));
//    QString cmd = "hwclock --set --date " + QString("\"%1%2%3\"").arg(vy, 4, 10, QChar('0')).arg(vm, 2, 10, QChar('0')).arg(vd, 2, 10, QChar('0'));
//    QString cmd = "sudo hwclock --set --date " + QString("\"%1%2%3\" ").arg(vy, 4, 10, QChar('0')).arg(vm, 2, 10, QChar('0')).arg(vd, 2, 10, QChar('0')) + QDateTime::currentDateTime().toString("hh:mm");
//    p.execute(cmd);

    QString dt =  "\""  + QString("%1%2%3 ").arg(vy, 4, 10, QChar('0')).arg(vm, 2, 10, QChar('0')).arg(vd, 2, 10, QChar('0')) +
                          QDateTime::currentDateTime().toString("hh:mm") +
                  "\"";

    QString atual = QDate::currentDate().toString("dd/MM/yyyy");
    QString nova  = QString("%3//%2//%1 ").arg(vy, 4, 10, QChar('0')).arg(vm, 2, 10, QChar('0')).arg(vd, 2, 10, QChar('0'));

    if( P11(tr("Data do sistema alterado de ") + atual + tr(" para ") + nova, true ) == false ) return;

    QString cmd;

    cmd = "sudo date --set " + dt;
    p.execute(cmd);
    p.waitForFinished(1000);

    cmd = "sudo hwclock --set --date " + dt;
    p.execute(cmd);
    p.waitForFinished(1000);

    // nano /etc/sudoers
    // ms ALL=(ALL) NOPASSWD: /bin/date
    // visao ALL=(ALL) NOPASSWD: /bin/date




    // sudo date +%Y%m%d -s "20180806"


}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_minus_hour_clicked()
{
    int v = ui->lbl_hour->text().toInt();
    if( v == 0 ) return;
    v--;
    ui->lbl_hour->setText(QString("%1").arg(v,2, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_plus_hour__clicked()
{
    int v = ui->lbl_hour->text().toInt();
    if( v == 23 ) return;
    v++;
    ui->lbl_hour->setText(QString("%1").arg(v,2, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_minus_min_clicked()
{
    int v = ui->lbl_min->text().toInt();
    if( v == 0 ) return;
    v--;
    ui->lbl_min->setText(QString("%1").arg(v,2, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_plus_min_clicked()
{
    int v = ui->lbl_min->text().toInt();
    if( v == 59 ) return;
    v++;
    ui->lbl_min->setText(QString("%1").arg(v,2, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_minus_y_clicked()
{
    int v = ui->lbl_y->text().toInt();

    if( v <= 2018 )
        v = 2018;
    else
        v--;

    ui->lbl_y->setText(QString("%1").arg(v, 4, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_plus_y_clicked()
{
    int v = ui->lbl_y->text().toInt();

    if( v > 2058 )
        return;
    else
        v++;

    ui->lbl_y->setText(QString("%1").arg(v, 4, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_minus_m_clicked()
{
    int v = ui->lbl_m->text().toInt();
    if( v == 1 ) return;
    v--;
    ui->lbl_m->setText(QString("%1").arg(v, 2, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_plus_m_clicked()
{
    int v = ui->lbl_m->text().toInt();
    if( v == 12 ) return;
    v++;
    ui->lbl_m->setText(QString("%1").arg(v, 2, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_minus_d_clicked()
{
    int v = ui->lbl_d->text().toInt();
    if( v == 1 ) return;
    v--;
    ui->lbl_d->setText(QString("%1").arg(v, 2, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_plus_d_clicked()
{
    int v = ui->lbl_d->text().toInt();
    if( v == 31 ) return;
    v++;
    ui->lbl_d->setText(QString("%1").arg(v, 2, 10, QChar('0')));
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_users_clicked()
{
    WindowManager::instance()->ShowScreen("Users");
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_database_clicked()
{
   if(P11(tr("Alterando configurações  do banco de dados"), true ) == false ) return;
   WindowManager::instance()->ShowScreen("ConfigDataBase");
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_maneger_output_clicked()
{
    if(P11(tr("Testando configurações  do saídas"), true ) == false ) return;
    WindowManager::instance()->ShowScreen("Saidas");
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_restart_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments()); //application restart
}

/// ===========================================================================
///
/// ===========================================================================
void FormConfig::on_btn_language_clicked()
{
    if(P11(tr("Alterando Idioma"), true ) == false ) return;
    WindowManager::instance()->ShowScreen("Language");
}
