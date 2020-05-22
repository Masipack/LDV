#ifndef FORMCONFIGUSER_H
#define FORMCONFIGUSER_H

#include <QWidget>
#include "interface/widgets/widgetuser.h"

namespace Ui {
class FormConfigUser;
}

class FormConfigUser : public QWidget
{
    Q_OBJECT

public:
    explicit FormConfigUser(QWidget *parent = 0);
    ~FormConfigUser();

protected:
    void showEvent(QShowEvent * event);

private slots:
    void on_btn_return_clicked();

private:
    Ui::FormConfigUser *ui;

    QVector<WidgetUser*>  widget_configuser;
};

#endif // FORMCONFIGUSER_H
