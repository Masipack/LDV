#include "dlgdatetime.h"
#include "ui_dlgdatetime.h"

#include <QScreen>
#include <QPushButton>

DlgDateTime::DlgDateTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDateTime)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog /*| Qt::X11BypassWindowManagerHint*/);

    month_names << tr("JANEIRO")   << tr("FEVEREIRO") << tr("MARÇO")
                << tr("ABRIL")     << tr("MAIO")      << tr("JUNHO")
                << tr("JULHO")     << tr("AGOSTO")    << tr("SETEMBRO")
                << tr("OUTRUBRO")  << tr("NOVEMBRO")  << tr("DEZEMBRO");


}

/// ===========================================================================
///
/// ===========================================================================
void DlgDateTime::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)

    current_month = QDate::currentDate().month()-1;
    current_year  = 0;
    current_day   = QDate::currentDate().day();

    ui->calendarWidget->setCurrentPage(QDate::currentDate().year() + current_year, current_month+1);

    ui->le_date->setText( month_names[current_month] + " " +
                          QString::number(QDate::currentDate().year() + current_year) );

}

DlgDateTime::~DlgDateTime()
{
    delete ui;
}

QString DlgDateTime::getText()
{
    QString a = QString("%1/%2/%3").arg(current_day, 2, 10, QChar('0')).arg(current_month+1, 2, 10, QChar('0')).arg(QDate::currentDate().year() + current_year, 4, 10, QChar('0'));
    QString b = QString(ui->le_hour->text() + ":" + ui->le_minute->text() + ":" + ui->le_second->text() );

    return (a + " " + b);
}


void DlgDateTime::on_btn_ok_clicked()
{
    this->accept();
}


void DlgDateTime::on_btn_cancel_clicked()
{
    this->reject();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgDateTime::on_btn_minus_month_clicked()
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
}

/// ===========================================================================
///
/// ===========================================================================
void DlgDateTime::on_btn_plus_month_clicked()
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
}

/// ===========================================================================
///
/// ===========================================================================
void DlgDateTime::on_calendarWidget_clicked(const QDate &date)
{
    current_day   = date.day();
}


void DlgDateTime::on_btn_minus_hour_clicked()
{
    int v = ui->le_hour->text().toInt();
    if( v < 1 ) return;
    v--;
    ui->le_hour->setText( QString("%1").arg(v, 2, 10, QChar('0')) );
}

void DlgDateTime::on_btn_plus_hour_clicked()
{
    int v = ui->le_hour->text().toInt();
    if( v > 22 ) return;
    v++;
    ui->le_hour->setText( QString("%1").arg(v, 2, 10, QChar('0')) );
}

void DlgDateTime::on_btn_minus_minute_clicked()
{
    int v = ui->le_minute->text().toInt();
    if( v < 1 ) return;
    v--;
    ui->le_minute->setText( QString("%1").arg(v, 2, 10, QChar('0')) );
}

void DlgDateTime::on_btn_plus_minute_clicked()
{
    int v = ui->le_minute->text().toInt();
    if( v > 58 ) return;
    v++;
    ui->le_minute->setText( QString("%1").arg(v, 2, 10, QChar('0')) );
}

void DlgDateTime::on_btn_minus_second_clicked()
{
    int v = ui->le_second->text().toInt();
    if( v < 1 ) return;
    v--;
    ui->le_second->setText( QString("%1").arg(v, 2, 10, QChar('0')) );
}

void DlgDateTime::on_btn_plus_second_clicked()
{
    int v = ui->le_second->text().toInt();
    if( v > 58 ) return;
    v++;
    ui->le_second->setText( QString("%1").arg(v, 2, 10, QChar('0')) );
}
