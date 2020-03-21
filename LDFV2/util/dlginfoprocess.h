#ifndef DLGINFOPROCESS_H
#define DLGINFOPROCESS_H

#include <QDialog>
#include <interface/windowmanager.h>
#include <interface/formprocess.h>

namespace Ui {
class dlgInfoProcess;
}

class dlgInfoProcess : public QDialog
{
    Q_OBJECT

public:
    explicit dlgInfoProcess(QWidget *parent = 0);
    ~dlgInfoProcess();
    enum ICON_TYPE{ IT_ERROR, IT_WARNING, IT_QUESTION, IT_INFO };
    void InitProcess(const QString& name);
    void SetMessage(ICON_TYPE it, const QString& msg);

private slots:
    void Close();
protected:
    void showEvent(QShowEvent *event);

private:
    Ui::dlgInfoProcess *ui;
    QString            name;
};

#endif // DLGINFOPROCESS_H
