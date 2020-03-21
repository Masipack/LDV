#include "dlgkeyboard.h"
#include "ui_dlgkeyboard.h"

#include <QScreen>

const char row1[10] = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' };
const char row2[11] = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '_', '/' };
const char row3[7] =  { 'Z', 'X', 'C', 'V', 'B', 'N', 'M' };

//const char numbers[12] = {'7','8','9','4','5','6','1','2','3','0'};
const char numbers[12] = {'1','2','3','4','5','6','7','8','9','0'};

int KEY_SZ = 40;

/// ===========================================================================
///
/// ===========================================================================
DlgKeyboard::DlgKeyboard(QWidget *parent) : QDialog(parent), ui(new Ui::DlgKeyboard)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);

    ui->setupUi(this);


    QScreen* screen = QGuiApplication::screens()[0];
    KEY_SZ = screen->geometry().width() / 24;

    numbers_only_max_len = -1;

    this->setMinimumWidth( 1280 * 0.9 );
    this->setMaximumWidth( 1280 * 0.9 );

    QString s;// = this->styleSheet();

    s += "border-color: rgb(0, 0, 0);\nborder-width: 3px;\nborder-radius: 5px; background-color: rgb(230, 230, 230);";

    this->setStyleSheet( s );

    QFont fnt("System",  screen->geometry().width() / 64 );

    for( int i = 0; i < 10; i++ )
    {
        QPushButton* btn = new QPushButton(this);
        btn->setText( QString( row1[i]) );
        btn->setMinimumWidth(KEY_SZ);
        btn->setMinimumHeight(KEY_SZ);
        btn->setMaximumWidth(KEY_SZ);
        btn->setMaximumHeight(KEY_SZ);
        btn->setFocusPolicy( Qt::NoFocus);
        btn->setStyleSheet( "background-color: rgb(200, 200, 200);");
        btn->setFont( fnt );
        ui->layout_1->insertWidget(i+1, btn);

        btn->setProperty("KEY", QChar( row1[i] ) );

        connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );

        bt_chars.push_back( btn );
    }

    QPushButton* btn = new QPushButton(this);
    btn->setText( QString( "<= DEL") );
    btn->setMinimumWidth(KEY_SZ*3);
    btn->setMinimumHeight(KEY_SZ);
    btn->setMaximumWidth(KEY_SZ*3);
    btn->setMaximumHeight(KEY_SZ);
    btn->setFocusPolicy( Qt::NoFocus);
    btn->setStyleSheet( "background-color: rgb(200, 200, 200);");
    btn->setFont( fnt );
    btn->setProperty("KEY", QChar( 0x01 ) );
    connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );
    ui->layout_1->insertWidget(11, btn);

    for( int i = 0; i < 11; i++ )
    {
        QPushButton* btn = new QPushButton(this);
        btn->setText( QString( row2[i]) );
        btn->setMinimumWidth(KEY_SZ);
        btn->setMinimumHeight(KEY_SZ);
        btn->setMaximumWidth(KEY_SZ);
        btn->setMaximumHeight(KEY_SZ);
        btn->setFocusPolicy( Qt::NoFocus);
        btn->setStyleSheet( "background-color: rgb(200, 200, 200);");
        btn->setFont( fnt );
        ui->layout_2->insertWidget(i+1, btn);
        btn->setProperty("KEY", QChar( row2[i] ) );
        connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );

        bt_chars.push_back( btn );
    }

    for( int i = 0; i < 7; i++ )
    {
        QPushButton* btn = new QPushButton(this);
        btn->setText( QString( row3[i]) );
        btn->setMinimumWidth(KEY_SZ);
        btn->setMinimumHeight(KEY_SZ);
        btn->setMaximumWidth(KEY_SZ);
        btn->setMaximumHeight(KEY_SZ);
        btn->setFocusPolicy( Qt::NoFocus);
        btn->setStyleSheet( "background-color: rgb(200, 200, 200);");
        btn->setFont( fnt );
        ui->layout_3->insertWidget(i+1, btn);
        btn->setProperty("KEY", QChar( row3[i] ) );
        connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );

        bt_chars.push_back( btn );
    }


    for( int i = 0; i < 10; i++ )
    {
        QPushButton* btn = new QPushButton(this);
        btn->setText( QString( numbers[i]) );
        btn->setMinimumWidth(KEY_SZ);
        btn->setMinimumHeight(KEY_SZ);
        btn->setMaximumWidth(KEY_SZ);
        btn->setMaximumHeight(KEY_SZ);
        btn->setFocusPolicy( Qt::NoFocus);
        btn->setStyleSheet( "background-color: rgb(200, 200, 200);");
        btn->setFont( fnt );
        ui->layout_num->insertWidget(i+1, btn);
        btn->setProperty("KEY", QChar( numbers[i] ) );
        connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );

        bt_chars.push_back( btn );
    }

    ui->pushButton->setMinimumWidth(KEY_SZ*8);
    ui->pushButton->setMinimumHeight(KEY_SZ);
    ui->pushButton->setMaximumWidth(KEY_SZ*8);
    ui->pushButton->setMaximumHeight(KEY_SZ);
    ui->pushButton->setProperty("KEY", QChar( ' ' ) );
    ui->pushButton->setStyleSheet( "background-color: rgb(200, 200, 200);");
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(keyPressed()) );
    bt_chars.push_back( ui->pushButton );
    ui->pushButton->setFont( fnt );

    ui->pushButton_2->setMinimumWidth(KEY_SZ*3);
    ui->pushButton_2->setMinimumHeight(KEY_SZ);
    ui->pushButton_2->setMaximumWidth(KEY_SZ*3);
    ui->pushButton_2->setMaximumHeight(KEY_SZ);
    ui->pushButton_2->setStyleSheet( "background-color: rgb(180, 180, 200);");
    ui->pushButton_2->setFont( fnt );

    ui->pushButton_3->setMinimumWidth(KEY_SZ*3);
    ui->pushButton_3->setMinimumHeight(KEY_SZ);
    ui->pushButton_3->setMaximumWidth(KEY_SZ*3);
    ui->pushButton_3->setMaximumHeight(KEY_SZ);
    ui->pushButton_3->setStyleSheet( "background-color: rgb(180, 180, 200);");
    ui->pushButton_3->setFont( fnt );

    ui->lineEdit->setMinimumHeight(KEY_SZ);
    ui->lineEdit->setMaximumHeight(KEY_SZ);
    ui->lineEdit->setFont( fnt );

    ui->btn_caps->setMinimumWidth(KEY_SZ*3);
    ui->btn_caps->setMinimumHeight(KEY_SZ);
    ui->btn_caps->setMaximumWidth(KEY_SZ*3);
    ui->btn_caps->setMaximumHeight(KEY_SZ);
    ui->btn_caps->setStyleSheet( "background-color: rgb(180, 180, 200);");
    ui->btn_caps->setFont( fnt );

}

/// ===========================================================================
///
/// ===========================================================================
DlgKeyboard::~DlgKeyboard()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void DlgKeyboard::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)
    QScreen *screen;
    if( QGuiApplication::screens().size() > 1  )
        screen = QGuiApplication::screens()[1];
    else
        screen = QGuiApplication::screens()[0];

    QRect r = this->geometry();
    r.moveCenter( screen->geometry().center() );
    this->setGeometry( r );

}

/// ===========================================================================
///
/// ===========================================================================
QString DlgKeyboard::getText()
{
    return ui->lineEdit->text();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgKeyboard::keyPressed()
{
    QObject* o = sender();
    QChar key = o->property("KEY").toChar();


    QString txt;

    if( key == QChar(0x01) )
    {
        txt = ui->lineEdit->text();
        txt.chop(1);
    }
    else
        txt = ui->lineEdit->text() + key;
    ui->lineEdit->setText(txt);
}

/// ===========================================================================
///
/// ===========================================================================
void DlgKeyboard::on_pushButton_2_clicked()
{
    this->accept();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgKeyboard::setNumbersOnly( int max_len )
{
    foreach ( QPushButton* btn, bt_chars)
    {
        btn->setEnabled(false);
    }


    numbers_only_max_len = max_len;
    ui->lineEdit->setMaxLength( max_len );
}

/// ===========================================================================
///
/// ===========================================================================
void DlgKeyboard::setPasswordMode( bool bEnable )
{
    if( bEnable )
        ui->lineEdit->setEchoMode(QLineEdit::Password);
    else
        ui->lineEdit->setEchoMode(QLineEdit::Normal);
}

/// ===========================================================================
///
/// ===========================================================================
void DlgKeyboard::on_pushButton_3_clicked()
{
    this->reject();
}

/// ===========================================================================
///
/// ===========================================================================
void DlgKeyboard::on_btn_caps_toggled(bool checked)
{
    foreach ( QPushButton* b, bt_chars)
    {
        if( b->text() == "SPACE" ) continue;

        if( checked )
            b->setText( b->text().toLower() );
        else
            b->setText( b->text().toUpper() );

        b->setProperty("KEY", b->text().at(0) );
    }
}
