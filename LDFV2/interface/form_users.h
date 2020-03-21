#ifndef FORM_USERS_H
#define FORM_USERS_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class FormUsers;
}

class FormUsers : public QWidget
{
    Q_OBJECT

public:
    explicit FormUsers(QWidget *parent = 0);
    ~FormUsers();

private:
    Ui::FormUsers *ui;

    QSqlQueryModel          model;
    QString                 str_query_all;
    QString                 str_query_active_only;

protected:
    void showEvent(QShowEvent * event);
    void UpdateTable();
private slots:
    void on_btn_menu_clicked();
    void on_btn_new_clicked();
    void on_btn_edit_clicked();
    void on_btn_delete_clicked();
    void on_btn_show_inactive_clicked();
};

#endif // FORM_USERS_H
