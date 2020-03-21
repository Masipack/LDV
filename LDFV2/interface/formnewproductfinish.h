#ifndef FORMNEWPRODUCTFINISH_H
#define FORMNEWPRODUCTFINISH_H

#include <QWidget>

namespace Ui {
class FormNewProductFinish;
}

class FormNewProductFinish : public QWidget
{
    Q_OBJECT

public:
    explicit FormNewProductFinish(QWidget *parent = 0);
    ~FormNewProductFinish();

    QString GetName();

signals:
    void Closed(const QString&);

private slots:
    void on_btn_finish_clicked();

private:
    Ui::FormNewProductFinish *ui;

protected:
    void showEvent(QShowEvent * event);
    bool eventFilter(QObject* object, QEvent* event);
};

#endif // FORMNEWPRODUCTFINISH_H
