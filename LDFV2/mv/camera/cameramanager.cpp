#include "cameramanager.h"

#include "mv/camera/cam_aca1300_60gc.h"

Q_GLOBAL_STATIC(CameraManager, cameraManager)

/// ===========================================================================
///
/// ===========================================================================
CameraManager *CameraManager::instance()
{
    return cameraManager();
}

/// ===========================================================================
///
/// ===========================================================================
CameraManager::~CameraManager()
{
    foreach (MvCamera* c, cameras.values())
    {
        if( c )
        {
            c->disconnect();
            c->stop();
            delete c;
        }
    }

    cameras.clear();
}

/// ===========================================================================
///
/// ===========================================================================
bool CameraManager::AddCamera(const QString& name, CAM_TYPE t)
{
    if( cameras.contains(name) ) return false;
    cameras.insert(name, new Cam_acA1300_60gc(name));
    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void CameraManager::ShutDown()
{
    foreach (MvCamera* c, cameras)
    {
        c->stop();
        c->wait( 100 );

        delete c;
    }
    cameras.clear();

}

/// ===========================================================================
///
/// ===========================================================================
MvCamera* CameraManager::GetCamera(const QString& name)
{
    if( cameras.contains(name) )
    {
        return cameras.value(name);
    }

    return NULL;
}
