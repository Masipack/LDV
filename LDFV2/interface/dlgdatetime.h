#ifndef DLGDATETIME_H
#define DLGDATETIME_H

#include <QDialog>
#include <QLayout>

namespace Ui {
class DlgDateTime;
}

class DlgDateTime : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDateTime(QWidget *parent = 0);
    ~DlgDateTime();

    QString getText();

private slots:
    void on_btn_ok_clicked();

    void on_btn_minus_month_clicked();

    void on_btn_plus_month_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_btn_cancel_clicked();

    void on_btn_minus_hour_clicked();

    void on_btn_plus_hour_clicked();

    void on_btn_minus_minute_clicked();

    void on_btn_plus_minute_clicked();

    void on_btn_minus_second_clicked();

    void on_btn_plus_second_clicked();

private:
    Ui::DlgDateTime *ui;

    QStringList     month_names;
    int             current_day;
    int             current_month;
    int             current_year;


protected:
    void showEvent(QShowEvent * event);
};

#endif // DLGDATETIME_H
