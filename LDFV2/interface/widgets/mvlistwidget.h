#ifndef MVLISTWIDGET_H
#define MVLISTWIDGET_H

#include <QWidget>
#include "global_defines.h"
#include "mv/tools/mvabstracttool.h"

namespace Ui {
class MVListWidget;
}

class MVListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MVListWidget(QString name, MV_TYPE type, QWidget *parent = nullptr);
    ~MVListWidget();

    void SetTool( MvAbstractTool* p );
    MvAbstractTool* GetTool( )              { return pTool; }

    QString GetName();
    MV_TYPE GetType() { return my_type; }

public slots:
    void NewResult(bool approved, const QString& value, quint32 proc_id);

private slots:
    void on_btn_up_clicked();

    void on_btn_dn_clicked();

private:
    Ui::MVListWidget *ui;

    MV_TYPE                 my_type;
    MvAbstractTool*         pTool;
};

#endif // MVLISTWIDGET_H
