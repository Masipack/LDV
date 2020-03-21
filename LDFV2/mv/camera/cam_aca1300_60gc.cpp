#include "cam_aca1300_60gc.h"
#include <QtCore>
#include <QtGui>

#include "util/systemsettings.h"
#include "util/sys_log.h"

#include <pylon/PylonIncludes.h>
#include <GenApi/Pointer.h>
#include <pylon/FeaturePersistence.h>

using namespace Pylon;
using namespace GenApi;

#include <opencv2/core.hpp>

/// ===========================================================================
///
/// ===========================================================================
Cam_acA1300_60gc::Cam_acA1300_60gc(const QString &camera, QObject *parent) : MvCamera(parent)
{
    this->name_camera = camera;
}

/// ===========================================================================
///
/// ===========================================================================
bool Cam_acA1300_60gc::SetGamma(bool bVal)
{
    try
    {
        if( pCamera == 0 ) return false;
        INodeMap& control = pCamera->GetNodeMap();
        SetNode( control.GetNode("GammaEnable"), bVal );
    }
    catch( const GenericException &e )
    {
        LOG(LOG_ERROR_TYPE, QString("Erro SetBlackLevel [%1]: %2").arg(camName).arg(e.GetDescription()) );
        return false;
    }
    return true;
}


/// ===========================================================================
///
/// ===========================================================================
bool Cam_acA1300_60gc::SetBlackLevel(int v)
{
    try
    {
        if( pCamera == 0 ) return false;
        INodeMap& control = pCamera->GetNodeMap();
        SetNode( control.GetNode("BlackLevelRaw"), v );
    }
    catch( const GenericException &e )
    {
        LOG(LOG_ERROR_TYPE, QString("Erro SetBlackLevel [%1]: %2").arg(camName).arg(e.GetDescription()) );
        return false;
    }
    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool Cam_acA1300_60gc::SetExposure(int v)
{
    try
    {
        if( pCamera == 0 ) return false;
        INodeMap& control = pCamera->GetNodeMap();
        SetNode( control.GetNode("ExposureTimeRaw"), v );
    }
    catch( const GenericException &e )
    {
        LOG(LOG_ERROR_TYPE, QString("Erro SetExposure [%1]: %2").arg(camName).arg(e.GetDescription()) );
        return false;
    }
    return true;
}

/// ===========================================================================
///
/// ===========================================================================
bool Cam_acA1300_60gc::LoadConfigs(Pylon::CInstantCamera* pCamera, bool bBack)
{
    if( bBack )
        CFeaturePersistence::Load("./data/back.pfs", &pCamera->GetNodeMap());
    else
    {
        QString Path = QString("./data/%1.pfs").arg(name_camera);
        CFeaturePersistence::Load(Path.toStdString().c_str(), &pCamera->GetNodeMap());
    }

    return true;
}
