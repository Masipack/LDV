#ifndef FORMNEWPRODUCTCONTENT_H
#define FORMNEWPRODUCTCONTENT_H

#include <QWidget>
#include <QTimer>
#include "mv/application/ibconfig.h"
#include "TO/productto.h"

namespace Ui {
class FormNewProductContent;
}

class FormNewProductContent : public QWidget
{
    Q_OBJECT

public:
    explicit FormNewProductContent(QWidget *parent = 0);
    ~FormNewProductContent();

    void SetCamera(const QString& name, int n);
    void StopCamera();
    bool GetTO(ProductTO &_TO);
    void StartTableDataBase();
    void StartKeepAlive();


public slots:
    void TimerColor();

private slots:
    void on_btn_add_clicked();

    void on_btn_remove_clicked();

    void on_btn_up_clicked();

    void on_btn_down_clicked();

    void on_lst_tools_currentRowChanged(int currentRow);

    void on_btn_camera_snap_clicked();

    void on_splitter_splitterMoved(int pos, int index);

    void on_btn_SincronizeDataBase_clicked();

private:
    Ui::FormNewProductContent *ui;

    QTimer      tmColor;
    int         index_color;
    int         index_color_dir;

    IBConfig    ib;


    QString         camName;
    int             camNum;


#ifdef _USE_DISK_
    QImage          test_image;
    QStringList     file_list;
    int             current_pos;
    QString         base_path;
    QString         file_ext;
#endif

protected:
    void showEvent(QShowEvent * event);
};

#endif // FORMNEWPRODUCTCONTENT_H
