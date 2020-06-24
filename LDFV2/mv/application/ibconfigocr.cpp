#include "ibconfigocr.h"

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
void IBConfigOCR::NewImage(const QImage &source)
{
    InspectionBuffer::NewImage(source);

    ocr_cfg->Exec(0);

    this->update();
}
