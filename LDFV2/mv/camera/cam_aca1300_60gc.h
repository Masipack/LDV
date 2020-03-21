#ifndef CAM_ACA1300_60GC_H
#define CAM_ACA1300_60GC_H

#include "mv/camera/mvcamera.h"

class Cam_acA1300_60gc : public MvCamera
{
    Q_OBJECT
public:
    explicit Cam_acA1300_60gc(const QString& camera,QObject *parent = 0);

    virtual bool LoadConfigs(Pylon::CInstantCamera* pCamera, bool bBack = false);

    bool SetGamma(bool bVal);

    bool SetBlackLevel(int v);

    bool SetExposure(int v);

signals:

public slots:
private:
    QString   name_camera;

};

#endif // CAM_ACA1300_60GC_H
