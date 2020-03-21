#include "dlgkeyboard.h"
#include "ui_dlgkeyboard.h"

#include <QScreen>

const char row1[10] = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' };
const char row2[10]  = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L' ,'/'};
const char row3[12] =  { 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '_', '@', '.' ,'(',')'};

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

    QScreen *screen;
    if( QGuiApplication::screens().size() > 1  )
        screen = QGuiApplication::screens()[1];
    else
        screen = QGuiApplication::screens()[0];

    KEY_SZ = screen->geometry().width() / 20;

    numbers_only_max_len = -1;

    QString s = "border-color: rgb(0, 0, 0); "
                "border-width: 3px; "
                "border-radius: 15px; "
                "background-color: rgb(210, 210, 210);";

    this->setStyleSheet( s );

    QString btn_style = "QPushButton { background-color: rgb(180, 180, 180); color: rgb(0, 0, 0); } "
                        "QPushButton::pressed { background-color: rgb(64, 64, 64); color: rgb(200, 200, 200); }";

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
        btn->setStyleSheet( btn_style );
        btn->setFont( fnt );
        ui->layout_1->insertWidget(i+1, btn);

        btn->setProperty("KEY", QChar( row1[i] ) );

        connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );

        bt_chars.push_back( btn );
    }

    for( int i = 0; i < 10; i++ )
    {
        QPushButton* btn = new QPushButton(this);
        btn->setText( QString( row2[i]) );
        btn->setMinimumWidth(KEY_SZ);
        btn->setMinimumHeight(KEY_SZ);
        btn->setMaximumWidth(KEY_SZ);
        btn->setMaximumHeight(KEY_SZ);
        btn->setFocusPolicy( Qt::NoFocus);
        btn->setStyleSheet( btn_style );
        btn->setFont( fnt );
        ui->layout_2->insertWidget(i+2, btn);
        btn->setProperty("KEY", QChar( row2[i] ) );
        connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );

        bt_chars.push_back( btn );
    }

    for( int i = 0; i < 12; i++ )
    {
        QPushButton* btn = new QPushButton(this);
        btn->setText( QString( row3[i]) );
        btn->setMinimumWidth(KEY_SZ);
        btn->setMinimumHeight(KEY_SZ);
        btn->setMaximumWidth(KEY_SZ);
        btn->setMaximumHeight(KEY_SZ);
        btn->setFocusPolicy( Qt::NoFocus);
        btn->setStyleSheet( btn_style );
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
        btn->setStyleSheet( btn_style );
        btn->setFont( fnt );
        ui->layout_num->insertWidget(i+1, btn);
        btn->setProperty("KEY", QChar( numbers[i] ) );
        connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );

        bt_chars.push_back( btn );
    }

    QPushButton* btn = new QPushButton(this);
    btn->setText( QString( "< DEL") );
    btn->setMinimumWidth(KEY_SZ*2+6);
    btn->setMinimumHeight(KEY_SZ);
    btn->setMaximumWidth(KEY_SZ*2+6);
    btn->setMaximumHeight(KEY_SZ);
    btn->setFocusPolicy( Qt::NoFocus);
    btn->setStyleSheet( btn_style );
    btn->setFont( fnt );
    btn->setAutoRepeat(true);
    btn->setAutoRepeatDelay(450);
    btn->setAutoRepeatInterval(150);
    btn->setProperty("KEY", QChar( 0x01 ) );
    connect(btn, SIGNAL(clicked()), this, SLOT(keyPressed()) );
    ui->layout_num->insertWidget(11, btn);


    ui->pushButton->setMinimumWidth(KEY_SZ*8);
    ui->pushButton->setMinimumHeight(KEY_SZ);
    ui->pushButton->setMaximumWidth(KEY_SZ*8);
    ui->pushButton->setMaximumHeight(KEY_SZ);
    ui->pushButton->setProperty("KEY", QChar( ' ' ) );
    ui->pushButton->setStyleSheet( btn_style );
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(keyPressed()) );
    bt_chars.push_back( ui->pushButton );
    ui->pushButton->setFont( fnt );

    QString style_special = "QPushButton::checked { background-color: rgb(48, 18, 28); color: rgb(190, 190, 200); } "
                            "QPushButton::pressed { background-color: rgb(18, 18, 28); color: rgb(180, 180, 190); } "
                            "QPushButton { background-color: rgb(50, 75, 100); color: rgb(230,230,230);}";

    ui->pushButton_2->setMinimumWidth(KEY_SZ*3);
    ui->pushButton_2->setMinimumHeight(KEY_SZ);
    ui->pushButton_2->setMaximumWidth(KEY_SZ*3);
    ui->pushButton_2->setMaximumHeight(KEY_SZ);
    ui->pushButton_2->setStyleSheet( style_special );
    ui->pushButton_2->setFont( fnt );

    ui->pushButton_3->setMinimumWidth(KEY_SZ*3);
    ui->pushButton_3->setMinimumHeight(KEY_SZ);
    ui->pushButton_3->setMaximumWidth(KEY_SZ*3);
    ui->pushButton_3->setMaximumHeight(KEY_SZ);
    ui->pushButton_3->setStyleSheet( style_special );
    ui->pushButton_3->setFont( fnt );

    ui->lineEdit->setMinimumHeight(KEY_SZ);
    ui->lineEdit->setMaximumHeight(KEY_SZ);
    ui->lineEdit->setFont( fnt );

    ui->btn_caps->setMinimumWidth(KEY_SZ*2);
    ui->btn_caps->setMinimumHeight(KEY_SZ);
    ui->btn_caps->setMaximumWidth(KEY_SZ*2);
    ui->btn_caps->setMaximumHeight(KEY_SZ);
    ui->btn_caps->setStyleSheet( style_special );
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
        QString s = btn->property("KEY").toString();
        if( s[0].isDigit() == false ) btn->setEnabled(false);
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
