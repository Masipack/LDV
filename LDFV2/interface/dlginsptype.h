#ifndef DLGINSPTYPE_H
#define DLGINSPTYPE_H

#include <QDialog>
#include "global_defines.h"

namespace Ui {
class DlgInspType;
}

class DlgInspType : public QDialog
{
    Q_OBJECT

public:
    explicit DlgInspType(QWidget *parent = 0);
    ~DlgInspType();

    QString GetName();
    MV_TYPE GetType();

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::DlgInspType *ui;

protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // DLGINSPTYPE_H
