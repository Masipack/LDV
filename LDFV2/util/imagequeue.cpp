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
bool ImageQueue::GetAt(int n, QImage &dest)
{
    if( queue_trainner.size() <= n ) return false;
    const QImage& d = queue_trainner.at( queue_trainner.size() - n - 1  );
    dest        = d;
    return true;
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
//    QUEUE_DATA d;
//    d.image = image;
//    d.data  = data;
//    queue.enqueue(d);
//    if( queue.size() > max_count ){

//        QUEUE_DATA c = queue.dequeue();

//    }

}

/// ===========================================================================
///
/// ===========================================================================
void ImageQueue::NewImageTrainner(const QImage &image)
{
//    queue_trainner.enqueue(image);

//    if(  queue_trainner.size() > 3 ){

//         QImage c =  queue_trainner.dequeue();
//    }
}

/// ===========================================================================
///
/// ===========================================================================
bool ImageQueue::GetAt(int n, QImage &dest, QString& dest_data)
{
    if( queue.size() <= n ) return false;
    const QUEUE_DATA& d = queue.at( queue.size() - n - 1  );
    dest        = d.image;
    dest_data   = d.data;
    return true;
}

/// ===========================================================================
///
/// ===========================================================================
void ImageQueue::RemoveLast()
{
    queue_trainner.pop_front();
}

/// ===========================================================================
///
/// ===========================================================================
void ImageQueue::Clear()
{
    queue.clear();
    queue_trainner.clear();

}
