#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

    QWidget* GetMainWidget();

public slots:
    void update_time();

private slots:
    void on_btn_login_clicked();

    void on_btn_alarm_clicked();

private:
    Ui::MainDialog *ui;

    QTimer  timer_date;
};

#endif // MAINDIALOG_H
