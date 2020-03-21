#ifndef FORMMAINMENU_H
#define FORMMAINMENU_H

#include <QWidget>

namespace Ui {
class FormMainMenuV;
class FormMainMenuH;
}

class FormMainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit FormMainMenu(QWidget *parent = 0);

signals:

public slots:

private slots:

    void on_btn_products_clicked();

    void on_btn_config_clicked();

    void on_btn_alarms_clicked();

    void on_btn_printer_clicked();

    void on_btn_export_data_clicked();

    void on_btn_part11_clicked();

    void on_btn_version_clicked();

private:
#ifdef _DISPLAY_HORIZONTAL_
    Ui::FormMainMenuH *ui;
#else
    Ui::FormMainMenuV *ui;
#endif

protected:
    void showEvent(QShowEvent * event);
    bool eventFilter(QObject* object, QEvent* event);
};

#endif // FORMMAINMENU_H
