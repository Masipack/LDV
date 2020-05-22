#ifndef PARAMSDATAMATRIX_H
#define PARAMSDATAMATRIX_H

#include <QWidget>

namespace Ui {
class ParamsDataMatrix;
}

class MvDataMatrix;

class ParamsDataMatrix : public QWidget
{
    Q_OBJECT

public:
    explicit ParamsDataMatrix(MvDataMatrix* p, QWidget *parent = 0);
    ~ParamsDataMatrix();

    void SetRunMode();

public slots:
    void NewResult(bool approved, const QString &value, quint32 proc_id);


private slots:
    void on_btn_visible_toggled(bool checked);

    void on_btn_angle_toggled(bool checked);

    void on_pushButton_clicked();

    void on_btn_position_toggled(bool checked);

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void showEvent(QShowEvent * event);

private:
    Ui::ParamsDataMatrix *ui;

    MvDataMatrix* pTool;
};

#endif // PARAMSDATAMATRIX_H
