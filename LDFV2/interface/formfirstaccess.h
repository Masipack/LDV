#ifndef FORMFIRSTACCESS_H
#define FORMFIRSTACCESS_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QModelIndex>


namespace Ui {
class FormFirstAccess;
}

class FormFirstAccess : public QWidget
{
    Q_OBJECT

public:
    explicit FormFirstAccess(QWidget *parent = 0);
    ~FormFirstAccess();

protected:
    void showEvent(QShowEvent * event);
    bool eventFilter(QObject* object, QEvent* event);


private slots:
    void on_btn_save_clicked();

private:
    Ui::FormFirstAccess *ui;

};

#endif // FORMFIRSTACCESS_H
