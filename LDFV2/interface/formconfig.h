#ifndef FORMCONFIG_H
#define FORMCONFIG_H

#include <QWidget>
#include "QProcess"
namespace Ui {
class FormConfigH;
class FormConfigV;
}

class FormConfig : public QWidget
{
    Q_OBJECT
public:
    explicit FormConfig(QWidget *parent = 0);

signals:

public slots:

private slots:
    void on_btn_menu_clicked();

    void on_btn_apply_time_clicked();

    void on_btn_apply_date_clicked();

    void on_btn_minus_hour_clicked();

    void on_btn_plus_hour__clicked();

    void on_btn_minus_min_clicked();

    void on_btn_plus_min_clicked();

    void on_btn_minus_y_clicked();

    void on_btn_plus_y_clicked();

    void on_btn_minus_m_clicked();

    void on_btn_plus_m_clicked();

    void on_btn_minus_d_clicked();

    void on_btn_plus_d_clicked();

    void on_btn_users_clicked();

    void on_btn_database_clicked();

    void on_btn_maneger_output_clicked();

    void on_btn_restart_clicked();

    void on_btn_language_clicked();

private:
#ifdef _DISPLAY_HORIZONTAL_
    Ui::FormConfigH *ui;
#else
    Ui::FormConfigV *ui;
#endif

     QProcess proc;

protected:
    void showEvent(QShowEvent * event);
};

#endif // FORMCONFIG_H
