#ifndef FORMVERSION_H
#define FORMVERSION_H

#include <QWidget>

namespace Ui {
class FormVersionH;
class FormVersionV;
}

class FormVersion : public QWidget
{
    Q_OBJECT
public:
    explicit FormVersion(QWidget *parent = 0);

signals:

public slots:

private slots:
    void on_btn_menu_clicked();

private:
#ifdef _DISPLAY_HORIZONTAL_
    Ui::FormVersionH *ui;
#else
    Ui::FormVersionV *ui;
#endif

protected:
    void showEvent(QShowEvent * event);
};

#endif // FORMVERSION_H
