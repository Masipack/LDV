#ifndef DLGDATABASE_H
#define DLGDATABASE_H

#include <QDialog>

namespace Ui {
class DlgDataBase;
}

class DlgDataBase : public QDialog
{
    Q_OBJECT

public:
    enum ICON_TYPE{ IT_ERROR, IT_WARNING, IT_QUESTION, IT_INFO };

    explicit DlgDataBase(QWidget *parent = 0);
    ~DlgDataBase();

     QString   GetLote();

protected:
    void showEvent(QShowEvent * event);

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::DlgDataBase *ui;

};

#endif // DLGDATABASE_H
