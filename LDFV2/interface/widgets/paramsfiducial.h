#ifndef PARAMSFIDUCIAL_H
#define PARAMSFIDUCIAL_H

#include <QWidget>

namespace Ui {
class ParamsFiducial;
}

class MvFiducial;

class ParamsFiducial : public QWidget
{
    Q_OBJECT

public:
    explicit ParamsFiducial(MvFiducial* p, QWidget *parent = 0);
    ~ParamsFiducial();

    void SetRunMode();

private slots:
    void on_btn_visible_toggled(bool checked);

    void on_btn_angle_toggled(bool checked);

    void on_btn_minus_similarity_clicked();

    void on_btn_plus_similarity_clicked();

    void on_btn_plus_region_clicked();

    void on_btn_minus_region_clicked();

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void showEvent(QShowEvent * event);

private:
    Ui::ParamsFiducial *ui;

    MvFiducial*  pTool;
};

#endif // PARAMSFIDUCIAL_H
