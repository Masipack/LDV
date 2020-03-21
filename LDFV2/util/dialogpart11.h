#ifndef DIALOGPART11_H
#define DIALOGPART11_H

#include <QDialog>

namespace Ui {
class DialogPart11;
}

class DialogPart11 : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPart11(QWidget *parent = 0);
    ~DialogPart11();

    QString GetUser();

    QString GetComment();

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::DialogPart11 *ui;

protected:
    void showEvent(QShowEvent * event);
    virtual bool eventFilter(QObject* object, QEvent* event);

};

#endif // DIALOGPART11_H
