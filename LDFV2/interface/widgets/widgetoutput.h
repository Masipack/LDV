#ifndef WIDGETOUTPUT_H
#define WIDGETOUTPUT_H

#include <QWidget>

namespace Ui {
class widgetOutPut;
}

class widgetOutPut : public QWidget
{
    Q_OBJECT

public:
    explicit widgetOutPut(QWidget *parent = 0);
    ~widgetOutPut();

     void SetNumberOut(const int n);
private slots:
     void on_btn_exec_toggled(bool checked);

private:
    Ui::widgetOutPut *ui;
};

#endif // WIDGETOUTPUT_H
