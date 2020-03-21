#ifndef FORMPRINTER_H
#define FORMPRINTER_H

#include <QWidget>

namespace Ui {
class FormPrinter;
}

class FormPrinter : public QWidget
{
    Q_OBJECT

public:
    explicit FormPrinter(QWidget *parent = 0);
    ~FormPrinter();

private slots:


    void on_btn_menu_clicked();

private:
    Ui::FormPrinter *ui;

protected:
    void showEvent(QShowEvent * event);
};

#endif // FORMPRINTER_H
