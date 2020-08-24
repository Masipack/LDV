#include "formtrainner.h"
#include "ui_formtrainner.h"
#include "interface/windowmanager.h"
#include "QFileDialog"
#include "util/serialcontrol.h"
#include "mv/camera/mvcamera.h"
#include "mv/camera/cameramanager.h"
#include "util/manegerlibrary.h"
#include "util/imagequeue.h"
#include "util/dlgkeyboard.h"


/// ===========================================================================
///
/// ===========================================================================
FormTrainner::FormTrainner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTrainner),current_pos(-1)
{
    ui->setupUi(this);

    currentIndex = -1;
    current_position_c=-1;

    dictionary = new ManegerLibrary();

    ui->image_device->setScene(&config_ocr);
    dictionary->LoadDictionary();

    connect(&config_ocr, SIGNAL(TemplateChanged()), this, SLOT(TemplateChanged()), Qt::UniqueConnection );

    ui->LE_CHAR->installEventFilter(this);

}

/// ===========================================================================
///
/// ===========================================================================
FormTrainner::~FormTrainner()
{
    delete ui;
    delete dictionary;

}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    WindowManager::instance()->SetInfoTop( "Treinamento de OCR's" );

    MvCamera* Camera = CameraManager::instance()->GetCamera("CAMERA_1");

    if( nullptr == Camera ) return;

    Camera->start("CAMERA_1");

    config_ocr.Reset();

    connect(Camera, SIGNAL(NewImage(QImage)), &config_ocr,SLOT(NewImage(QImage)), Qt::UniqueConnection );

    int pos = 0;
    QImage img;

    if( ImageQueue::instance()->GetAt(pos, img) == false ) return;
    config_ocr.NewImage(img);
    ui->lbl_pos->setText( QString::number( pos) );

}


/// ===========================================================================
///
/// ===========================================================================
bool FormTrainner::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->LE_CHAR && event->type() == QEvent::MouseButtonRelease )
    {
        ui->LE_CHAR->setText( "" );
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->LE_CHAR->setText( dlg.getText() );
        }
        return false;
    }

    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::Snap()
{

#ifndef _USE_DISK_
    SerialControl::instance()->SendCommand( SerialControl::COMMAND_1116_FRONT, 1 );
    SerialControl::instance()->SendCommand( SerialControl::COMMAND_1116_SNAP,  1 );
    qDebug() << Q_FUNC_INFO << __LINE__;
#else

    GetConfig(base_path, "DISK/BASE_PATH", QString("") );
    GetConfig(file_ext , "DISK/FILE_EXT" , QString("") );

    int rot = 0;
    GetConfig(rot , "DISK/ROTATE_ANG", 0 );

    QString fmt = "RGB";
    GetConfig(fmt , "DISK/DEST_FORMAT" , QString("RGB") );

    if( file_list.isEmpty() )
    {
//            base_path   = "/home/ms/Pictures/blister/02092017/IFA";
//            file_ext    = "bmp";

        if( base_path.at(base_path.size()-1) == '/' ) base_path.remove(base_path.size()-1, 1);

        file_list = GetFileList(base_path, file_ext);
        current_pos = 0;
    }
    else
    {
        QString fname(base_path + "/" + file_list.at(current_pos) + "." + file_ext);
        cv::Mat A = cv::imread(fname.toLatin1().data());

        if( rot == 90 )
        {
            uchar depth = A.type() & CV_MAT_DEPTH_MASK;
            uchar chans = 1 + (A.type() >> CV_CN_SHIFT);
            if( fmt != "RGB" )
            {
                if( depth == CV_8U && chans == 3)   cv::cvtColor(A, A, cv::COLOR_RGB2GRAY);
                if( depth == CV_8U && chans == 4)   cv::cvtColor(A, A, cv::COLOR_RGBA2GRAY);
            }
            cv::transpose(A, A);
            cv::flip(A, A,1);
        }


        test_image = QImage(A.cols, A.rows, QImage::Format_RGB888 );

        for( int i = 0; i < A.rows; i++)
        {
            memcpy( &test_image.bits()[ test_image.bytesPerLine() * i ], A.row(i).data, A.cols * A.channels() );
        }


       // ibBlister.NewImage( test_image );
       config_ocr.NewImage( test_image );

        current_pos++;
        if( current_pos == file_list.size() ) current_pos = 0;

    }
#endif

}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::TemplateChanged()
{
    current_position_c = -1;
    currentContour.image = QImage();
    currentContour.character = QChar();
    ui->LBL_IMAGE->clear();
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_PrevImage_clicked()
{
    int pos = ui->lbl_pos->text().toInt();
    if( pos == 0 ) return;

    pos--;
    current_position_c = pos;
    QImage img;
    QString s;
    if( ImageQueue::instance()->GetAt(pos, img) == false ) return;
    config_ocr.NewImage(img);
    ui->lbl_pos->setText( QString::number( pos) );
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_NextImage_clicked()
{
    int pos = ui->lbl_pos->text().toInt();
    pos++;

    current_position_c = pos;
    QImage img;
    QString s;
    if( ImageQueue::instance()->GetAt(pos, img) == false ) return;
    config_ocr.NewImage(img);
    ui->lbl_pos->setText( QString::number( pos) );
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_menu_clicked()
{
    MvCamera* Camera = CameraManager::instance()->GetCamera("CAMERA_1");
    if( nullptr == Camera ) return;
    Camera->disconnect();
    Camera->stop();

    config_ocr.Reset();

    WindowManager::instance()->ShowLastScreen();
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_Next_clicked()
{

    ui->LE_CHAR->clear();
    ui->LE_CHAR->setFocus();

    int temp_index = current_position_c + 1;

    if( config_ocr.GetItem(temp_index, currentContour) )
    {
        current_position_c  = temp_index;
        ui->LBL_IMAGE->setPixmap( QPixmap::fromImage(currentContour.image) );
        if( currentContour.character.isNull() == false ) ui->LE_CHAR->setText( currentContour.character );
        config_ocr.SetCurrentItem( current_position_c  );
        config_ocr.update();
    }
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_Prev_clicked()
{
    if( current_position_c <= 0 ) return;

    ui->LE_CHAR->clear();
    ui->LE_CHAR->setFocus();

    int temp_index = current_position_c  - 1;

    if( config_ocr.GetItem(temp_index, currentContour) )
    {
        current_position_c  = temp_index;
        ui->LBL_IMAGE->setPixmap( QPixmap::fromImage(currentContour.image) );
        if( currentContour.character.isNull() == false ) ui->LE_CHAR->setText( currentContour.character );
        config_ocr.SetCurrentItem( current_position_c);
        config_ocr.update();
    }

}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_Pluss_clicked()
{
    ui->LE_CHAR->setFocus();

    if( ui->LE_CHAR->text().isEmpty() || (current_position_c ==-1)) return;

    config_ocr.SetItem(current_position_c, ui->LE_CHAR->text().at(0) );

    dictionary->SetItem(config_ocr.GetCONTOUR(current_position_c));

    on_btn_Next_clicked();
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_Min_clicked()
{
  dictionary->RemoveItem(currentIndex);

}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_Save_clicked()
{
  dictionary->Train();

  MvCamera* Camera = CameraManager::instance()->GetCamera("CAMERA_1");
  if( nullptr == Camera ) return;
  Camera->disconnect();
  Camera->stop();

  dictionary->SaveDictionary("./data/hog_library/");

  P11("--------- Treinamento de Caractere --------- ",true);
  WindowManager::instance()->ShowLastScreen();

}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_background_toggled(bool checked)
{
    config_ocr.setBackground(checked?1:-1);
    config_ocr.Exec();
    ui->btn_background->setText(checked ? "Fundo escuro": "Fundo claro");
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_Next_caracter_clicked()
{
    int temp_index = currentIndex  + 1;

    if( dictionary->GetItem(temp_index, currentContour_Caracter) )
    {
        currentIndex  = temp_index;

        ui->lb_current_caracter->setPixmap( QPixmap::fromImage(currentContour_Caracter.image) );
        ui->lb_current_caracter->setStyleSheet("background-color: rgb(0, 0, 0);border-radius:10px;");
        if( currentContour_Caracter.character.isNull() == false ) ui->LE_CARACTER->setText( currentContour_Caracter.character );

    }
}

/// ===========================================================================
///
/// ===========================================================================
void FormTrainner::on_btn_Prev_caracter_clicked()
{
    if( currentIndex <= 0 ) return;

    int temp_index = currentIndex  - 1;

    if( dictionary->GetItem(temp_index, currentContour_Caracter) )
    {
        currentIndex  = temp_index;

        ui->lb_current_caracter->setPixmap( QPixmap::fromImage(currentContour_Caracter.image) );
        ui->lb_current_caracter->setStyleSheet("background-color: rgb(0, 0, 0);border-radius:10px;");
        if( currentContour_Caracter.character.isNull() == false ) ui->LE_CARACTER->setText( currentContour_Caracter.character );

    }
}

