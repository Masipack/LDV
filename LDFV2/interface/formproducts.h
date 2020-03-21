#ifndef FORMPRODUCTS_H
#define FORMPRODUCTS_H

#include <QWidget>

namespace Ui {
class FormProductsH;
class FormProductsV;
}

class FormProducts : public QWidget
{
    Q_OBJECT
public:
    explicit FormProducts(QWidget *parent = 0);

signals:

public slots:

private slots:
    void on_btn_menu_clicked();

    void on_btn_up_clicked();

    void on_btn_down_clicked();

    void on_btn_delete_clicked();

    void on_btn_new_clicked();

    void on_btn_load_clicked();

private:
#ifdef _DISPLAY_HORIZONTAL_
    Ui::FormProductsH *ui;
#else
    Ui::FormProductsV *ui;
#endif

protected:
    bool eventFilter(QObject* object, QEvent* event);
    void showEvent(QShowEvent * event);

};

#endif // FORMPRODUCTS_H
