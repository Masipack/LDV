#include "maindialog.h"
#include <QApplication>

#include <pylon/PylonIncludes.h>

#include "util/sys_log.h"
#include "util/systemsettings.h"
#include "util/serialcontrol.h"
#include "interface/windowmanager.h"
#include "mv/camera/cameramanager.h"
#include "util/alarmmanager.h"
#include "util/msqlsingleton.h"
#include "util/fileutil.h"
#include "TO/databaseto.h"
#include "util/systemsettings.h"
#include <AVL.h>
#include "opencv2/core.hpp"

Q_DECLARE_METATYPE(cv::Mat)



/// ===========================================================================
///
/// ===========================================================================
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);

    QFile outFile("./log/" + QDate::currentDate().toString("yyyy-MM-dd") + ".log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    if( !outFile.isOpen() ) return; // cant open  log !

    QTextStream s(&outFile);
    s.setCodec("UTF-8");
    s << QTime::currentTime().toString( "hh:mm:ss.zzz") << QString::fromUtf8( msg.toUtf8().data() ) << "\r\n";

    outFile.waitForBytesWritten(1);
    outFile.flush();
    outFile.close();
}

/// ===========================================================================
///
/// ===========================================================================
bool InitSettings()
{
    if( !QFile("./data/settings.dat").exists() )
    {
        if( !QDir("./data").exists() )   QDir().mkdir("./data");
        LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: ARQUIVO DE CONFIGURACOES NAO ENCONTRADO. VERIFIQUE A INSTALACAO DO SISTEMA, SISTEMA DESLIGANDO"));
        return false;
    }
    if( SystemSettings::instance()->init("./data/settings.dat") == false )
    {
        LOG(LOG_ERROR_TYPE, QObject::tr("ERRO FATAL: ARQUIVO DE CONFIGURACOES NAO ENCONTRADO. VERIFIQUE A INSTALACAO DO SISTEMA, SISTEMA DESLIGANDO"));
        return false;
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void InitLog()
{
    if( !QDir("./log").exists() )   QDir().mkdir("./log");
    qInstallMessageHandler(customMessageHandler);
    LOG(LOG_INFO_TYPE, QObject::tr(" --------- Sistema Inicializando --------- ") );
}

/// ===========================================================================
///
/// ===========================================================================
bool setStyle(const QString& file, QApplication* a )
{
    // Application Appearance
    QFile f(file);
    if (!f.exists())
    {
        LOG(LOG_ERROR_TYPE, QObject::tr("Arquivo de estilos nao encontrado") );
        return false;
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        if( ! f.isOpen() )
        {
            LOG(LOG_WARNING_TYPE, QObject::tr("Arquivo de estilos corrompido"));
            return false;
        }

        QTextStream ts(&f);
        a->setStyleSheet(ts.readAll());

    }
    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool InitComPort()
{
    QString comPort;
    int tempInt;

#ifdef __linux__
    GetConfig(comPort, "SYSTEM/COM_PORT", QString("ttyS0"));
#else
    GetConfig(comPort, "SYSTEM/COM_PORT", QString("com1"));
#endif

    GetConfig(tempInt, "SYSTEM/COM_BAUDRATE", 19200);
    if( SerialControl::instance()->init(comPort, tempInt) == false )
    {
        LOG(LOG_ERROR_TYPE, QObject::tr("Porta serial ") + comPort + QObject::tr(" indisponivel para uso") );
        return false;
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool InitDatabase()
{

    bool user_p11 = false;
    bool user_database = false;
    bool user_printer= false;

    QString drive;

    GetConfig(user_p11, "SYSTEM/USE_PART11", false);

    GetConfig(user_database, "SYSTEM/USE_DATABASE", false);
    GetConfig(drive, "SYSTEM/DRIVE", QString("ODBC Driver 17 for SQL Server"));

    GetConfig(user_printer, "SYSTEM/USE_PRINTER", false);

    qApp->setProperty("USE_DATABASE",user_database);
    qApp->setProperty("USE_PART11", user_p11);
    qApp->setProperty("USE_PRINTER", user_printer);


    if( user_p11 )
    {
        QSqlDatabase::addDatabase("QSQLITE", "PART11");
        QSqlDatabase::database("PART11").setDatabaseName("Audit.db");
        if (!QSqlDatabase::database("PART11").open())
        {
            LOG(LOG_ERROR_TYPE, QObject::tr("Arquivo de dados PART11 nao encontrado") );
            return false;
        }
    }


    QSqlDatabase::addDatabase("QSQLITE", "SYSTEM");
    QSqlDatabase::database("SYSTEM").setDatabaseName("./data/system.dat");

    if (!QSqlDatabase::database("SYSTEM").open())
    {
        LOG(LOG_ERROR_TYPE, QObject::tr("Arquivo de dados do sistema nao encontrado") );
        return false;
    }

    //---------------- Leitura parametros do database para comunicação com servidor externo-------//
    DATABASE   database;
    DATABASETO databaseto;

    QString error;

    if(!ReadGenericTO(databaseto,"./data/database/ldfbd.bin",error)) {
        LOG(LOG_ERROR_TYPE, QObject::tr("%1").arg(error));
    }

    ConvertDATABASETO_To_DATABASE(database,databaseto);


    if (user_database)
    {
        if(database.IsEmpty() || database.IsNull()) return false ;

        MSQLSingleton::instance()->Init(database,drive);
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool GetTranslation(QTranslator& translate)
{

    QString s_lang, s_locale;

    GetConfig(s_lang    , "SYSTEM/LANGUAGE"     , QString("pt-br") );
    GetConfig(s_locale  , "SYSTEM/LOCALE"       , QString("br") );
    Debug(s_lang)

    if(s_lang=="bs")
    {
        return translate.load(":/bosnian.qm");
    }

    if(s_lang=="en-US")
    {
        return translate.load(":/englesh_en.qm");
    }


    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool InitCameras()
{
    QString camText;
    GetConfig(camText, "CAMERAS/CAMERA_LIST", QString("CAMERA_1") );
    QStringList camList( camText.split(',') );
    for(int i = 0; i < camList.size(); i++)
    {
        CameraManager::instance()->AddCamera(camList.at(i), CameraManager::ACA1300_U60GC);
    }
    return true;
}

/// ===========================================================================
///
///
/// ===========================================================================
int main(int argc, char *argv[])
{

    //---------------------------- Registers Types --------------------------//

    qRegisterMetaType<cv::Mat>("cv::Mat");



    int currentExitCode = 0;
    bool bInitError = false;


    QApplication a(argc, argv);

    //----------------------------- Init Settings ---------------------------//

    if( InitSettings() == false                             ) bInitError = true;

    //----------------------------- Pylon ------------------------------------//

    Pylon::PylonAutoInitTerm    autoInitTerm;

    //------------- First initialize AVL's inner state------------------------//

    avl::InitLibrary();


    QTranslator translate;

    if(GetTranslation(translate)==false) bInitError = true;

     a.installTranslator(&translate);


#ifndef _TESTE_
    InitLog();
#endif

    if( InitComPort() == false                              ) bInitError = true;
    if( InitDatabase() == false                             ) bInitError = true;
    if( setStyle(":/styles/style_base.qss", &a) == false    ) bInitError = true;
    if( WindowManager::instance()->init() == false          ) bInitError = true;
    if( InitCameras() == false                              ) bInitError = true;

    if( bInitError ) AlarmManager::instance()->SetAlarm(ALM_INIT);

    P11(QObject::tr("Inicializando Sistema"));

    currentExitCode = a.exec();


    CameraManager::instance()->ShutDown();

    LOG(LOG_INFO_TYPE, QObject::tr(" --------- Sistema Desligando --------- ") );

    return currentExitCode;


}
