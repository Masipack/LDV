#ifndef FORMPROCESS_H
#define FORMPROCESS_H

#include <QWidget>
#include "interface/formprocesscontent.h"
#include "util/dlginfo.h"
#include "TO/multicamto.h"

namespace Ui {
class FormProcess;
}

class FormProcess : public QWidget
{
    Q_OBJECT

public:
    explicit FormProcess(QWidget *parent = 0);
    ~FormProcess();

    bool Init( const QString& file_name );
    void DeInit();

signals:
    void Close();
private slots:
    void on_btn_btn_prev_clicked();

    void on_btn_next_clicked();

    void on_btn_return_clicked();
    void on_btn_printer_clicked();

protected:
    void showEvent(QShowEvent * event);

public slots:
    void WriteConfig(const QString& name);

private:
    Ui::FormProcess *ui;

    DlgInfo dlg;

    QVector<FormProcessContent*>    forms;
    int current_form;
    QString Name;


    bool            use_PRINTER;
    bool            b_save_without_msg;
    MultiCamTO     to;


};

#endif // FORMPROCESS_H
