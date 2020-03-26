#ifndef DLGINFO_H
#define DLGINFO_H

#include <QDialog>

namespace Ui {
class DlgInfo;
}

class DlgInfo : public QDialog
{
    Q_OBJECT

public:
    enum ICON_TYPE{ IT_ERROR, IT_WARNING, IT_QUESTION, IT_INFO };

    explicit DlgInfo(QWidget *parent = 0);
    ~DlgInfo();

    void SetVisible(const bool value);
    void SetMessage(ICON_TYPE it, const QString& msg, bool bShowCancel = false, bool bYesNo = false);

protected:
    void showEvent(QShowEvent * event);

private slots:
    void on_btn_ok_clicked();
    void on_btn_cancel_clicked();

private:
    Ui::DlgInfo *ui;

};

#endif // DLGINFO_H
