#include "mvcamera.h"
#include <QtCore>
#include <QtGui>

#include "util/systemsettings.h"
#include "util/sys_log.h"

#include <pylon/PylonIncludes.h>
#include <GenApi/Pointer.h>

using namespace Pylon;
using namespace GenApi;

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "util/alarmmanager.h"

/// ===========================================================================
///
/// ===========================================================================
MvCamera::MvCamera(QObject *parent) : QThread(parent)
{
    pCamera         = 0;
    rotationAngle   = 0;
    bBacklight = false;
}

/// ===========================================================================
///
/// ===========================================================================
void MvCamera::start(const QString &name, QThread::Priority p)
{
    bStop           = false;
    camName         = name;
    QThread::start(p);
}

/// ===========================================================================
///
/// ===========================================================================
bool MvCamera::stop()
{
    bStop = true;
    if( this->wait(1000) == false ) return false;
    pCamera = 0;
    bBacklight = false;
    return  true;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvCamera::SetNode( GenApi::INode* p, int v )
{
    if( p == NULL )
    {
        LOG( LOG_WARNING_TYPE, tr("ERRO: parametro inexistente" ) );
        return false;
    }

    try
    {
        if( IsWritable( p ) == false )
        {
            LOG( LOG_WARNING_TYPE, tr("Atencao: parametro inacessivel: " ) + QString(p->GetName()) );
            return false;
        }

        CIntegerPtr(p)->SetValue( v );

    }
    catch( const GenericException &e )
    {
        LOG(LOG_ERROR_TYPE, QString("Erro na captura de imagens [%1]: %2").arg(camName).arg(e.GetDescription()) );
        return false;
    }

    return true;
}



/// ===========================================================================
///
/// ===========================================================================
bool MvCamera::SetNode( GenApi::INode* p , bool v)
{
    if( p == NULL )
    {
        LOG( LOG_WARNING_TYPE, tr("ERRO: parametro inexistente" ) );
        return false;
    }

    try
    {
        if( IsWritable( p ) == false )
        {
            LOG( LOG_WARNING_TYPE, tr("Atencao: parametro inacessivel: " ) + QString(p->GetName()) );
            return false;
        }

        CBooleanPtr(p)->SetValue( v );

    }
    catch( const GenericException &e )
    {
        LOG(LOG_ERROR_TYPE, QString("Erro na captura de imagens [%1]: %2").arg(camName).arg(e.GetDescription()) );
        return false;
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvCamera::SetNode( GenApi::INode* p , const QString& v)
{
    if( p == NULL )
    {
        LOG( LOG_WARNING_TYPE, tr("ERRO: parametro inexistente" ) );
        return false;
    }

    try
    {
        if( IsWritable( p ) == false || IsAvailable( p ) == false || IsImplemented( p ) == false )
        {
            LOG( LOG_WARNING_TYPE, tr("Atencao: parametro inacessivel: %1" ).arg( QString(p->GetName() ) ) );
            return false;
        }

        CEnumerationPtr(p)->FromString( v.toLatin1().data() );

    }
    catch( const GenericException &e )
    {
        LOG(LOG_ERROR_TYPE, QString("Erro na captura de imagens [%1]: %2 %3").arg(camName).arg(e.GetDescription()).arg(QString(p->GetName())) );
        return false;
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool MvCamera::SetNode( GenApi::INode* p , float v)
{
    if( p == NULL )
    {
        LOG( LOG_WARNING_TYPE, tr("ERRO: parametro inexistente" ) );
        return false;
    }

    try
    {
        if( IsWritable( p ) == false )
        {
            LOG( LOG_WARNING_TYPE, tr("Atencao: parametro inacessivel: " ) + QString(p->GetName()) );
            return false;
        }

        CFloatPtr(p)->SetValue( v );

    }
    catch( const GenericException &e )
    {
        LOG(LOG_ERROR_TYPE, QString("Erro na captura de imagens [%1]: %2").arg(camName).arg(e.GetDescription()) );
        return false;
    }

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void MvCamera::run()
{
    DeviceInfoList list;
    Pylon::CTlFactory& TlFactory = CTlFactory::GetInstance();

    TlFactory.EnumerateDevices( list );

    if( list.size() == 0 )
    {
        LOG(LOG_ERROR_TYPE, tr("Erro ao inicializar cameras: Lista de cameras vazia") );
        AlarmManager::instance()->SetAlarm(ALM_CAMERA_ERROR);
        return;
    }

    int cam_index = -1;
    for( uint i = 0; i < list.size(); i++ )
    {
        if( camName.compare( list.at(i).GetUserDefinedName().c_str() ) == 0 )
        {
            cam_index = i;
            break;
        }
    }

    if( cam_index == -1 )
    {
        LOG(LOG_ERROR_TYPE, tr("Camera %1 nao encontrada").arg(camName) );
        AlarmManager::instance()->SetAlarm(ALM_CAMERA_ERROR);
        return;
    }

    //Criando objeto de câmera...
    CInstantCamera camera( CTlFactory::GetInstance().CreateDevice( list.at( cam_index ) ) );
    try
    {
        camera.Open();
    }
    catch( const GenericException &e )
    {
        LOG(LOG_ERROR_TYPE, tr("Erro ao abrir camera  [%1]: %2").arg(camName).arg(e.GetDescription()) );
        AlarmManager::instance()->SetAlarm(ALM_CAMERA_ERROR);
        return;
    }

    pCamera = &camera;

    if( LoadConfigs( &camera, bBacklight ) == false )
    {
        LOG(LOG_ERROR_TYPE, tr("Erro carregando parametros da camera %1").arg(camName) );
        AlarmManager::instance()->SetAlarm(ALM_CAMERA_ERROR);
        return;
    }

    camera.StartGrabbing();

    CGrabResultPtr ptrGrabResult;
    QImage         imageQt;

    //CLog::ShutDown();

    bStop = false;
    while( bStop == false )
    {
        try
        {
            camera.RetrieveResult( 100, ptrGrabResult, TimeoutHandling_ThrowException );
        }
        catch( const TimeoutException &e )
        {
            continue;
        }
        catch( const GenericException &e )
        {
            LOG(LOG_ERROR_TYPE, tr("Erro timeout  na captura da imagem [%1]: %2").arg(camName).arg(e.GetDescription()) );
            AlarmManager::instance()->SetAlarm(ALM_CAMERA_ERROR);
            break;
        }


        if( ptrGrabResult->GrabSucceeded() ) // se aquisicao de imagem da camera foi OK
        {
            // Seta tamanho da imagem, compativel com o recebido da camera
            if( (quint32)imageQt.width() != ptrGrabResult->GetWidth() ||  (quint32)imageQt.height() != ptrGrabResult->GetHeight() )
            {
                int w = ptrGrabResult->GetWidth();
                int h = ptrGrabResult->GetHeight();

                if( rotationAngle == 90 || rotationAngle == 270  )
                {
                    w = ptrGrabResult->GetHeight();
                    h = ptrGrabResult->GetWidth();
                }

                QImage::Format format = QImage::Format_Indexed8;
                if( ptrGrabResult->GetPixelType() != PixelType_Mono8 ) format = QImage::Format_RGB888;

                // Cria imagem 'Qt' baseado no tamanho recebido
                imageQt = QImage( w, h, format );
            }

            // Copia para o endereco da imagem 'Qt', a imagem 'Basler' recebida
            if( ptrGrabResult->GetPixelType() == PixelType_Mono8 )
            {
                if( rotationAngle == 0 )
                {
                    memcpy( imageQt.bits(), ptrGrabResult->GetBuffer(), imageQt.bytesPerLine() * imageQt.height() );
                }
                else
                {
                    cv::Mat A( cv::Size(ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight()), CV_8UC1, (char*)ptrGrabResult->GetBuffer() );

                    if( rotationAngle == 90 )
                    {
                        transpose(A, A);
                        flip(A, A,1);
                    }

                    if( rotationAngle == 180  )
                    {

                    }

                    if( rotationAngle == 270  )
                    {

                    }

                    for( int i = 0; i < A.rows; i++)
                    {
                        memcpy( &imageQt.bits()[ imageQt.bytesPerLine() * i ], A.row(i).data, A.cols * A.channels() );
                    }
                }

            }
            else
            {
                cv::Mat A;
                cv::Mat B;

                A = cv::Mat( cv::Size(ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight()), CV_8UC1, (char*)ptrGrabResult->GetBuffer() );
                cv::cvtColor(A, B, cv::COLOR_BayerBG2BGR_VNG );

                if( rotationAngle == 90 )
                {
                    transpose(B, A);
                    flip(A, B, 1);
                }

                for( int i = 0; i < B.rows; i++)
                {
                    memcpy( &imageQt.bits()[ imageQt.bytesPerLine() * i ], B.row(i).data, B.cols * B.channels() );
                }

            }

            emit( NewImage(imageQt) );
        }
        else
        {
            LOG(LOG_ERROR_TYPE, tr("Erro no resultado da captura de imagens") );
            AlarmManager::instance()->SetAlarm(ALM_CAMERA_ERROR);
            break;
        }
    }

    camera.StopGrabbing();
    camera.Close();
    bBacklight = false;
    pCamera = 0;

}
