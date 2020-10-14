#ifndef FORMPROCESSCONTENT_H
#define FORMPROCESSCONTENT_H

#include <QWidget>
#include "mv/application/ibprocess.h"
#include "TO/productto.h"
#include "interface/formstatistics.h"

namespace Ui {
class FormProcessContent;
}

class FormProcessContent : public QWidget
{
    Q_OBJECT

public:
    explicit FormProcessContent(QWidget *parent = 0);
    ~FormProcessContent();

    bool SetTO(const ProductTO& src);
    void SetFileName(const QString& name){configFilename = name;}

    void SetCamera(const QString& name, int n);
    void StopCamera();
    bool GetTO(ProductTO &_TO);
    void SetOutput(const int& value){ ib.SetOutput(value);}
    bool ChangesTOOLS(ProductTO &other);
    void SetStatistic(const bool& v);
    void ResetScene();




signals:
    void WriteConfig(const QString&);
public slots:
    void TimerColor();
    void SimulationProcess();
    void on_lst_tools_currentRowChanged(int currentRow);

private:
    QTimer      tmColor;
    int         index_color;
    int         index_color_dir;

    Ui::FormProcessContent *ui;

    IBProcess   ib;

    FormStatistics*    frmStatistics ;

    QTimer             timerKeepAlive;



    QString         camName;
    int             camNum;

    ProductTO       ProdutoTOChanges;
    QString         configFilename;

#ifdef _USE_DISK_
    QImage          test_image;
    QStringList     file_list;
    int             current_pos;
    QString         base_path;
    QString         file_ext;
#endif

protected:
    void showEvent(QShowEvent * event);

private slots:
    void on_btn_camera_snap_clicked();
    void on_splitter_splitterMoved(int pos, int index);
    void on_btn_Statistics_toggled(bool checked);
    void on_btn_ImageLog_clicked();
    void on_btn_sincrinizeDataBase_clicked();
};

#endif // FORMPROCESSCONTENT_H
