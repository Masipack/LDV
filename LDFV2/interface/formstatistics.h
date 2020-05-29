#ifndef FORMSTATISTICS_H
#define FORMSTATISTICS_H

#include <QWidget>

namespace Ui {
class FormStatistics;
}

class FormStatistics : public QWidget
{
    Q_OBJECT

public:
    explicit FormStatistics(QWidget *parent = 0);
    ~FormStatistics();

public slots:
    void NewInsp(bool v);

private slots:
    void on_btn_reset_clicked();
protected:
    void showEvent(QShowEvent * event);

private:
    Ui::FormStatistics *ui;
};

#endif // FORMSTATISTICS_H
