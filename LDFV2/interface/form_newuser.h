#ifndef FORM_NEWUSER_H
#define FORM_NEWUSER_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QModelIndex>

namespace Ui {
class FormNewUser;
}

class FormNewUser : public QWidget
{
    Q_OBJECT

public:
    explicit FormNewUser(QWidget *parent = 0);
    ~FormNewUser();

    bool SetDialogData(QSqlQueryModel& model, QModelIndex& mi);

    void SetNewUserMode();

private slots:
    void on_btn_menu_clicked();

    void on_btn_new_clicked();

    void on_btn_dec_level_clicked();

    void on_btn_inc_level_clicked();

private:
    Ui::FormNewUser *ui;

    void ClearScreen();

    QStringList user_level_names;

    int current_user_level;

    bool bEditMode;

protected:
    void showEvent(QShowEvent * event);
    bool eventFilter(QObject* object, QEvent* event);
};

#endif // FORM_NEWUSER_H
