#include "imagequeue.h"

#include <QDebug>

Q_GLOBAL_STATIC(ImageQueue, umage_queue)

/// ===========================================================================
///
/// ===========================================================================
ImageQueue *ImageQueue::instance()
{
    return umage_queue();
}

/// ===========================================================================
///
/// ===========================================================================
ImageQueue::~ImageQueue()
{
}

/// ===========================================================================
///
/// ===========================================================================
void ImageQueue::NewImage(const QImage& image, const QString& data)
{
    QUEUE_DATA d;
    d.image = image;
    d.data  = data;
    queue.enqueue(d);
    if( queue.size() > max_count ) queue.dequeue();
}

/// ===========================================================================
///
/// ===========================================================================
bool ImageQueue::GetAt(int n, QImage& dest, QString &dest_data)
{
    if( queue.size() <= n ) return false;
    const QUEUE_DATA& d = queue.at( queue.size() - n - 1  );
    dest        = d.image;
    dest_data   = d.data;
    return true;
}
