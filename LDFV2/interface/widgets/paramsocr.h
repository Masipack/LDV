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

    void SetTableDataBase(const QList<QString> attributes);
    void SetExpectedText(const QString  text);
    void SetBackgroundStateTool(const int& v);
    void SetRotationStateTool(const bool& v);

     
public slots:
    void NewResult(bool approved, const QString &value, quint32 proc_id);
    void NewResultAttributes(const QString &value);
    
signals:
    void NewAttribute(const QString value);

private slots:
    void on_btn_visible_toggled(bool checked);

    void on_btn_angle_toggled(bool checked);

    void on_btn_minus_white_clicked();

    void on_btn_plus_white_clicked();

    void on_btn_show_result_toggled(bool checked);

    void on_pushButton_clicked();

    void on_cb_database_currentTextChanged(const QString &value);
    
    void on_btn_black_white_clicked(bool checked);

    void on_btn_edit_position_toggled(bool checked);

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void showEvent(QShowEvent * event);

private:
    Ui::ParamsOCR *ui;
    MvOCR*  pTool;
    int     white_filter;

};

#endif // PARAMSOCR_H
