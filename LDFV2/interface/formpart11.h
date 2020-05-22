#ifndef FORMPART11_H
#define FORMPART11_H

#include <QWidget>

namespace Ui {
class FormPart11;
}

class FormPart11 : public QWidget
{
    Q_OBJECT

public:
    explicit FormPart11(QWidget *parent = 0);
    ~FormPart11();

private slots:
    void on_btn_menu_clicked();

    void on_btn_filter_clicked();

    void on_btn_export_pdf_clicked();

    void on_btn_clean_clicked();

    void on_btn_backupCRF21_clicked();

private:
    Ui::FormPart11 *ui;

protected:
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent * event);
    bool eventFilter(QObject* object, QEvent* event);

private:
    QString GetQuery();

    QString strPedido;
    QString PATH_FILE;
};

#endif // FORMPART11_H
