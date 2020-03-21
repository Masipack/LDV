#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <QtCore>
#include "mv/camera/mvcamera.h"

class CameraManager : public QObject
{
    Q_OBJECT
public:
    enum CAM_TYPE
    {
        ACA1300_U60GC   = 0x01,
        ACA1300_U60GM   = 0x02
    };

    explicit CameraManager(QObject *parent = 0) { Q_UNUSED(parent); }
    virtual ~CameraManager();

    static CameraManager *instance();

    bool AddCamera(const QString& name, CAM_TYPE t);
    MvCamera* GetCamera(const QString& name);

    void ShutDown();

signals:

public slots:

private:
    QMap<QString, MvCamera*>    cameras;
};

#endif // CAMERAMANAGER_H
