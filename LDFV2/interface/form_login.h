#ifndef FORM_LOGIN_H
#define FORM_LOGIN_H

#include <QWidget>
#include <QMap>

namespace Ui {
class FormLogin;
}

class FormLogin : public QWidget
{
    Q_OBJECT

public:
    explicit FormLogin(QWidget *parent = 0);
    ~FormLogin();

private slots:
    void on_btn_return_clicked();

    void on_btn_login_clicked();

    void on_btn_logoff_clicked();

private:
    Ui::FormLogin *ui;
    QMap<QString, int> error_count;

protected:
    void showEvent(QShowEvent * event);
    bool eventFilter(QObject* object, QEvent* event);
};

#endif // FORM_LOGIN_H
