#ifndef FORMSAIDAS_H
#define FORMSAIDAS_H

#include <QWidget>
#include "interface/widgets/widgetoutput.h"
namespace Ui {
class FormSaidas;
}

class FormSaidas : public QWidget
{
    Q_OBJECT

public:
    explicit FormSaidas(QWidget *parent = 0);
    ~FormSaidas();

private slots:
    void on_btn_return_clicked();

protected:
    void showEvent(QShowEvent * event);

private:

    Ui::FormSaidas *ui;

    QVector<widgetOutPut*>  widget_output;
};

#endif // FORMSAIDAS_H
