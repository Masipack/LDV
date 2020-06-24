#include "windowmanager.h"

#include <QDebug>
#include <QtGui>
#include <QtWidgets>
#include <QScreen>

#include "util/systemsettings.h"

#include "maindialog.h"
#include "interface/formmainmenu.h"
#include "interface/form_login.h"
#include "interface/formalarms.h"
#include "interface/formusbexport.h"
#include "interface/formversion.h"
#include "interface/formprinter.h"
#include "interface/formpart11.h"
#include "interface/form_users.h"
#include "interface/form_newuser.h"
#include "interface/formconfig.h"
#include "interface/formproducts.h"
#include "interface/formnewproduct.h"
#include "interface/formnewproductfinish.h"
#include "interface/formprocess.h"
#include "interface/formimagelog.h"
#include "interface/configdatabase.h"
#include "interface/formsaidas.h"
#include "interface/formfirstaccess.h"
#include "interface/formconfiguser.h"
#include "interface/formlanguage.h"
#include "interface/formtrainner.h"



Q_GLOBAL_STATIC(WindowManager, windowManager)

/// ===========================================================================
///
/// ===========================================================================
WindowManager *WindowManager::instance()
{
    return windowManager();
}

/// ===========================================================================
///
/// ===========================================================================
WindowManager::~WindowManager()
{
}

/// ===========================================================================
///
/// ===========================================================================
bool WindowManager::init()
{
    user_level_names << tr("OPERADOR") << tr("SUPERVISOR") << tr("ADMINISTRADOR");

    lbl_top     = 0;
    lbl_bottom  = 0;
    lbl_user    = 0;
    lbl_level   = 0;

    current_screen      = 0;
    current_user_level  = -1;
    current_user_login  = "SISTEMA";

    qApp->setProperty("CURRENT_USER", current_user_login);

    QScreen *screen;

#ifndef _TESTE_
    screen = QGuiApplication::screens()[0];
#else
    if( QGuiApplication::screens().size() > 1  )
        screen = QGuiApplication::screens()[1];
    else
        screen = QGuiApplication::screens()[0];
#endif

#ifdef _DISPLAY_HORIZONTAL_
    screen_orientation = SCREEN_HORIZONTAL;
#else
    screen_orientation = SCREEN_VERTICAL;
#endif

    mainDialog = new MainDialog();
    mainDialog->setWindowFlags( Qt::FramelessWindowHint | Qt::Window /*| Qt::X11BypassWindowManagerHint*/ | Qt::WindowStaysOnTopHint  ); // TODO: recolocar (ver windows32)

#ifndef _TESTE_
    mainDialog->move(screen->geometry().x(), screen->geometry().y());
    mainDialog->resize(screen->geometry().width(), screen->geometry().height());
    mainDialog->showFullScreen();
#else
    if( screen_orientation == SCREEN_HORIZONTAL )
    {
        mainDialog->move(0,0);
        mainDialog->resize(1024,768);
        mainDialog->setMinimumSize(1024,768);
        mainDialog->setMaximumSize(1024,768);
    }
    else
    {
        mainDialog->move(0,0);
        mainDialog->resize(800,1024);
    }
    mainDialog->show();
#endif

    GetConfig(logoff_minute_count, "SYSTEM/LOGOFF_MINUTES", 10);
    mainWidget = mainDialog->GetMainWidget();
    qApp->installEventFilter( this );
    connect(&logoff_timer, SIGNAL(timeout()), this, SLOT(LogOff()) );
    logoff_timer.start( 1000 * 60 * logoff_minute_count );

    return LoadScreens();
}

/// ===========================================================================
///
/// ===========================================================================
bool WindowManager::eventFilter(QObject* object, QEvent* event)
{
    if( event->type() == QEvent::MouseButtonRelease )
    {
        logoff_timer.start( 1000 * 60 * logoff_minute_count );
    }
    return false;
}

/// ===========================================================================
///
/// ===========================================================================
void WindowManager::LogOff()
{
    P11(tr("Logoff automático de usuario:") + current_user_login );
    WindowManager::instance()->SetCurrentUser(tr("SISTEMA"), -1);

    if( current_screen )
    {
        mainWidget->layout()->removeWidget( current_screen );
        current_screen->close();
        mainWidget->layout()->addWidget( current_screen );
        current_screen->show();
    }

    QWidget* p = v_screens.value("MainMenu");

    if( p == NULL ) return ;
    if( current_screen->objectName() == p->objectName() ) return;

    DlgInfo dlg;
    dlg.SetMessage(DlgInfo::IT_INFO, tr("LOGOUT automático realize o login !"), false, false );
    dlg.exec();
  //  ShowScreen("Login");

    logoff_timer.stop();
}

/// ===========================================================================
///
/// ===========================================================================
bool WindowManager::LoadScreens()
{
    QWidget* a = new FormMainMenu( mainWidget );
    if( a ) v_screens.insert( "MainMenu"                  , a );

    a = new FormLogin(mainWidget);
    if( a ) v_screens.insert( "Login"                     , a );

    a = new FormAlarms(mainWidget);
    if( a ) v_screens.insert( "Alarms"                    , a );

    a = new FormUsbExport(mainWidget);
    if( a ) v_screens.insert( "ExportData"                , a );

    a = new FormVersion(mainWidget);
    if( a ) v_screens.insert( "Version"                   , a );

    a = new FormPrinter(mainWidget);
    if( a ) v_screens.insert( "Printer"                   , a );

    a = new FormPart11(mainWidget);
    if( a ) v_screens.insert( "P11"                       , a );

    a = new FormUsers(mainWidget);
    if( a ) v_screens.insert( "Users"                     , a );

    a = new FormNewUser(mainWidget);
    if( a ) v_screens.insert( "NewUser"                   , a );

    a = new FormConfig(mainWidget);
    if( a ) v_screens.insert( "Config"                    , a );

    a = new FormProducts(mainWidget);
    if( a ) v_screens.insert( "Products"                  , a );

    a = new FormNewProduct(mainWidget);
    if( a ) v_screens.insert( "NewProduct"                , a );

    a = new FormProcess(mainWidget);
    if( a ) v_screens.insert( "Process"                   , a );

    a = new ConfigDataBase(mainWidget);
    if( a ) v_screens.insert( "ConfigDataBase"            , a );

    a = new FormImageLog(mainWidget);
    if( a ) v_screens.insert( "ImageLog"                  , a );


    a = new FormSaidas(mainWidget);
    if( a ) v_screens.insert( "Saidas"                   , a );

    a = new FormFirstAccess(mainWidget);
    if( a ) v_screens.insert( "FirstAccess"               , a );

    a = new FormConfigUser(mainWidget);
    if( a ) v_screens.insert( "ConfigUser"               , a );


    a = new FormLanguage(mainWidget);
    if( a ) v_screens.insert( "Language"                 , a );

    a = new FormTrainner(mainWidget);
    if( a ) v_screens.insert( "Trainne"                 , a );


    QWidget* p = v_screens.value("MainMenu");
    if( p == NULL ) return false;

    mainWidget->layout()->addWidget( p );
    current_screen = p;
    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void WindowManager::SetInfoTop(const QString& info)
{
    if( lbl_top )
    {
        lbl_top->setText( info );
    }
}

/// ===========================================================================
///
/// ===========================================================================
void WindowManager::SetInfoBottom(const QString& info)
{
    if( lbl_bottom )
    {
        lbl_bottom->setText( info );
    }
}

/// ===========================================================================
///
/// ===========================================================================
bool WindowManager::ShowScreen(const QString& window_name)
{
    QWidget* p = v_screens.value(window_name);
    if( p == NULL ) return false;

    if( current_screen->objectName() == p->objectName() ) return true;

    last_screen.push( current_screen );
    if( last_screen.size() > 8 ) last_screen.removeAt(0);

    if( current_screen )
    {
        mainWidget->layout()->removeWidget( current_screen );
        current_screen->close();
        current_screen = 0;
    }

    current_screen = p;
    mainWidget->layout()->addWidget( current_screen );
    current_screen->show();

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool WindowManager::ShowLastScreen()
{
    if( last_screen.isEmpty() || last_screen.top() == NULL  )
    {
        return false;
    }

    if( current_screen )
    {
        mainWidget->layout()->removeWidget( current_screen );
        current_screen->close();
        current_screen = 0;
    }

    current_screen = last_screen.top();
    mainWidget->layout()->addWidget( current_screen );
    current_screen->show();
    last_screen.pop();

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
QWidget* WindowManager::GetScreen(const QString& window_name)
{
    QWidget* p = v_screens.value(window_name);
    if( p == nullptr ) return nullptr;

    return p;
}

/// ===========================================================================
///
/// ===========================================================================
void WindowManager::SetCurrentUser(const QString& login, int level)
{
    if( lbl_user )  lbl_user->setText( login );
    if( lbl_level )
    {
        if( level >= 0 && level < user_level_names.size() )
        {
            lbl_level->setText( user_level_names.at(level) );
        }

        if( level == -1 )
        {
            lbl_level->setText(tr("Efetuar Login"));
        }
    }

    qApp->setProperty("CURRENT_USER", login);

    current_user_login = login;
    current_user_level = level;
}

