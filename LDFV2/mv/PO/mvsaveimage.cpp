#include "mvsaveimage.h"


#include "util/imagequeue.h"
#include  <QPainter>

/// ===========================================================================
///
/// ===========================================================================
MvSaveImage::MvSaveImage(QObject *parent):QObject(parent)
{

}

/// ===========================================================================
///
/// ===========================================================================
MvSaveImage::~MvSaveImage()
{

}

/// ===========================================================================
///
/// ===========================================================================
void MvSaveImage::SetListErros(QString &erros)
{
   result_summary = erros;
}

/// ===========================================================================
///
/// ===========================================================================
void MvSaveImage::Exec(QGraphicsScene *SceneCurrent, QImage image)
{
    img_inspection_result = QImage(image.size(), QImage::Format_ARGB32);
    img_inspection_result.convertToFormat( QImage::Format_ARGB32 );
    img_inspection_result.fill(Qt::transparent);

    QPainter painter(&img_inspection_result);

    SceneCurrent->render(&painter,image.rect(), image.rect(),
                              Qt::KeepAspectRatio  );

    ImageQueue::instance()->NewImage(img_inspection_result, result_summary);
}
