#ifndef MVCAMERA_H
#define MVCAMERA_H

#ifdef _WIN32
#define NOMINMAX
#endif

#include <QThread>
#include "pylon/InstantCamera.h"
#include <GenApi/INode.h>

class MvCamera : public QThread
{
    Q_OBJECT
public:
    explicit MvCamera(QObject *parent = 0);

    virtual bool LoadConfigs(Pylon::CInstantCamera* pCamera, bool bBack) = 0;

    virtual bool SetGamma(bool bVal) = 0;

    virtual bool SetBlackLevel(int v) = 0;

    virtual bool SetExposure(int v) = 0;

    void SetBacklight( bool bVal ) { bBacklight = bVal; }


signals:
    void NewImage(const QImage& image);

public slots:
    void start(const QString& name, QThread::Priority p = InheritPriority );
    bool stop();

protected:
    virtual void run();

    bool SetNode(GenApi::INode* p , int            v);
    bool SetNode(GenApi::INode* p , bool           v);
    bool SetNode(GenApi::INode* p , const QString& v);
    bool SetNode(GenApi::INode *p , float          v);

    bool                        bStop;
    QString                     camName;
    Pylon::CInstantCamera*      pCamera;
    int                         rotationAngle;

    bool                        bBacklight;

};

#endif // MVCAMERA_H
