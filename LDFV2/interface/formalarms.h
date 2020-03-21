#ifndef FORMALARMS_H
#define FORMALARMS_H

#include <QWidget>

namespace Ui {
class FormAlarms;
}

class FormAlarms : public QWidget
{
    Q_OBJECT

public:
    explicit FormAlarms(QWidget *parent = nullptr);
    ~FormAlarms();

public slots:
    void NewAlarmList(QStringList data);

private slots:

    void on_btn_alarms_toggled(bool checked);

    void on_btn_logs_toggled(bool checked);

    void on_btn_return_clicked();

    void on_btn_reset_alarms_clicked();

    void on_btn_minus_month_clicked();

    void on_btn_plus_month_clicked();

    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::FormAlarms *ui;

    QStringList     month_names;
    int             current_day;
    int             current_month;
    int             current_year;


    void UpdateLogList();

protected:
    void showEvent(QShowEvent * event);

};

#endif // FORMALARMS_H
