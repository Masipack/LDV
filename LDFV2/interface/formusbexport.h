#ifndef FORMUSBEXPORT_H
#define FORMUSBEXPORT_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class FormUsbExport;
}

class FormUsbExport : public QWidget
{
    Q_OBJECT

public:
    explicit FormUsbExport(QWidget *parent = nullptr);
    ~FormUsbExport();

public slots:
    void UpdateDeviceName();

private:
    Ui::FormUsbExport*  ui;
    QTimer              timerUpdateName;
    QString             usbPath;
    QString             strPedido;

protected:
    void showEvent(QShowEvent * event);

private slots:
    void on_btn_return_clicked();
    void on_btn_from_usb_clicked();
    void on_btn_to_usb_clicked();
    void on_btn_config_from_usb_clicked();
    void on_btn_config_to_usb_clicked();
    void on_btn_to_usb_all_clicked();
};

#endif // FORMUSBEXPORT_H
