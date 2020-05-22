#include "ibimagelog.h"

#include <QGraphicsView>
#include <QtCore>
#include "util/systemsettings.h"
#include "util/imagequeue.h"

using namespace cv;

/// ===========================================================================
///
/// ===========================================================================
IBImageLog::IBImageLog(QObject *parent) : InspectionBuffer(parent)
{
    this->SetRunMode(Processing);


}

/// ===========================================================================
///
/// ===========================================================================
void IBImageLog::NewImage(const QImage &source)
{
    InspectionBuffer::NewImage(source);
    this->update();
}


/// ===========================================================================
///
/// ===========================================================================
void IBImageLog::Reset()
{
    currentImage = QImage();
    mat = cv::Mat();
    this->update();
}
