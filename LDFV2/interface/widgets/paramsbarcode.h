#ifndef PARAMSBARCODE_H
#define PARAMSBARCODE_H

#include <QWidget>

namespace Ui {
class ParamsBarcode;
}

class MvBarcode;

class ParamsBarcode : public QWidget
{
    Q_OBJECT

public:
    explicit ParamsBarcode(MvBarcode* p, QWidget *parent = 0);
    ~ParamsBarcode();

     void SetRunMode();

public slots:
    void NewResult(bool approved, const QString &value, quint32 proc_id);

private slots:
    void on_btn_visible_toggled(bool checked);

    void on_btn_angle_toggled(bool checked);

    void on_btn_show_result_toggled(bool checked);

    void on_btn_pharmacode_toggled(bool checked);

    void on_btn_pharmacode_dir_toggled(bool checked);

    void on_pushButton_clicked();

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void showEvent(QShowEvent * event);

private:
    Ui::ParamsBarcode *ui;

    MvBarcode* pTool;

};

#endif // PARAMSBARCODE_H
