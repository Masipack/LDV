#ifndef FORMLANGUAGE_H
#define FORMLANGUAGE_H

#include <QWidget>

namespace Ui {
class FormLanguage;
}

class FormLanguage : public QWidget
{
    Q_OBJECT

public:
    explicit FormLanguage(QWidget *parent = 0);
    ~FormLanguage();

private slots:
    void on_btn_return_clicked();

    void on_btn_ENG_clicked();

    void on_btn_PT_clicked();

    void on_btn_BN_clicked();

private:
    Ui::FormLanguage *ui;
};

#endif // FORMLANGUAGE_H
