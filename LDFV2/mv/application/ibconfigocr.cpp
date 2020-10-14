#include "ibconfigocr.h"
#include  "util/serialcontrol.h"

using namespace cv;

/// ===========================================================================
///
/// ===========================================================================
IBConfigOCR::IBConfigOCR(QObject *parent) : InspectionBuffer(parent)
{
    this->SetRunMode(Interactive);

    ocr_cfg = new MvOCRConfig(QRectF(100,100, 300,200),nullptr);
    ocr_cfg->SetLockAngle( true );
    ocr_cfg->SetResizeOnCenter( false );
    ocr_cfg->SetExecOnMove( true );

    this->addItem( ocr_cfg );

    connect(ocr_cfg, SIGNAL(TemplateChanged()), this, SIGNAL(TemplateChanged()), Qt::QueuedConnection );

    connect(&timerKeepAlive, SIGNAL(timeout()), this, SLOT(SendKeepAlive()) );
}

/// ===========================================================================
///
/// ===========================================================================
IBConfigOCR::~IBConfigOCR()
{
    delete ocr_cfg;
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfigOCR::Reset()
{
    mat = cv::Mat();
    currentImage = QImage();
    update();
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfigOCR::Init()
{
 timerKeepAlive.start();
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfigOCR::Stop()
{
 timerKeepAlive.stop();
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfigOCR::NewImage(const QImage &source)
{
    InspectionBuffer::NewImage(source);

    ocr_cfg->Exec(0);

    this->update();
}

/// ===========================================================================
///
/// ===========================================================================
void IBConfigOCR::SendKeepAlive()
{
    SerialControl::instance()->KeepAlive();
}
