#ifndef FORMNEWPRODUCT_H
#define FORMNEWPRODUCT_H

#include <QWidget>
#include <QTimer>
#include "mv/application/ibconfig.h"
#include "interface/formnewproductcontent.h"
#include "interface/formnewproductfinish.h"

namespace Ui {
class FormNewProduct;
}

class FormNewProduct : public QWidget
{
    Q_OBJECT

public:
    explicit FormNewProduct(QWidget *parent = 0);
    ~FormNewProduct();

    void Init();
    void DeInit();

public slots:
    void WriteConfig(const QString& name);

private slots:


    void on_btn_btn_prev_clicked();

    void on_btn_next_clicked();

    void on_btn_return_clicked();

private:
    Ui::FormNewProduct *ui;

    QVector<FormNewProductContent*> forms;
    int current_form;
    FormNewProductFinish*           formFinish;


protected:
    void showEvent(QShowEvent * event);
};

#endif // FORMNEWPRODUCT_H
