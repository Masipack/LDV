#ifndef FORMTRAINNER_H
#define FORMTRAINNER_H

#include <QWidget>
#include "util/manegerlibrary.h"
#include "mv/application/ibconfigocr.h"
#include "mv/tools/mvocrconfig.h"
#include "util/systemsettings.h"
#include "util/fileutil.h"


namespace Ui {
class FormTrainner;
}

class FormTrainner : public QWidget
{
    Q_OBJECT

public:
    explicit FormTrainner(QWidget *parent = nullptr);
    ~FormTrainner();

protected:
    void showEvent(QShowEvent * event);
    bool eventFilter(QObject* object, QEvent* event);

public slots:
    void Snap();
    void TemplateChanged();

private slots:
    void on_btn_PrevImage_clicked();

    void on_btn_NextImage_clicked();

    void on_btn_menu_clicked();

    void on_btn_Next_clicked();

    void on_btn_Prev_clicked();

    void on_btn_Pluss_clicked();

    void on_btn_Min_clicked();

    void on_btn_Save_clicked();

    void on_btn_background_toggled(bool checked);


    void on_btn_Next_caracter_clicked();

    void on_btn_Prev_caracter_clicked();


private:
      Ui::FormTrainner *ui;

      MvOCRPO::DESCRIPTOR    currentContour;
      int                    currentIndex;

      MvOCRPO::DESCRIPTOR    currentContour_Caracter;

      IBConfigOCR            config_ocr;

      ManegerLibrary*        dictionary;
      int                    current_pos;
      int                    current_position_c;


      QImage          test_image;
      QStringList     file_list;
      QString         base_path;
      QString         file_ext;




};

#endif // FORMTRAINNER_H
