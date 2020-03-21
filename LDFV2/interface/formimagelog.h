#ifndef FORMIMAGELOG_H
#define FORMIMAGELOG_H

#include <QWidget>
#include "util/imagequeue.h"
#include "interface/windowmanager.h"


namespace Ui {
class FormImageLog;
}

class FormImageLog : public QWidget
{
    Q_OBJECT

public:
    explicit FormImageLog(QWidget *parent = nullptr);
    ~FormImageLog();

private slots:
    void on_btn_prev_clicked();

    void on_btn_next_clicked();

    void on_btn_menu_clicked();

private:
    Ui::FormImageLog *ui;

protected:
    void showEvent(QShowEvent * event);

};

#endif // FORMIMAGELOG_H
