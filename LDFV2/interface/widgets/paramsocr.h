#ifndef PARAMSOCR_H
#define PARAMSOCR_H

#include <QWidget>


namespace Ui {
class ParamsOCR;
}

class MvOCR;

class ParamsOCR : public QWidget
{
    Q_OBJECT

public:
    explicit ParamsOCR(MvOCR*  p, QWidget *parent = 0);
    ~ParamsOCR();

public slots:
    void NewResult(bool approved, const QString &value, quint32 proc_id);

private slots:
    void on_btn_visible_toggled(bool checked);

    void on_btn_angle_toggled(bool checked);

    void on_btn_minus_white_clicked();

    void on_btn_plus_white_clicked();

    void on_btn_show_result_toggled(bool checked);

    void on_pushButton_clicked();

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void showEvent(QShowEvent * event);

private:
    Ui::ParamsOCR *ui;
    MvOCR*  pTool;
    int white_filter;
};

#endif // PARAMSOCR_H
