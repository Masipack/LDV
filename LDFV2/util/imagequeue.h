#ifndef IMAGEQUEUE_H
#define IMAGEQUEUE_H

#include <QObject>
#include <QQueue>
#include <QImage>

class ImageQueue : public QObject
{
    Q_OBJECT
public:
    inline ImageQueue(QObject *parent = 0) { Q_UNUSED(parent); max_count = 10; }
    ~ImageQueue();

    static ImageQueue *instance();

    void SetMaxCount(int n) { max_count = n; }

    bool GetAt(int n, QImage &dest);

    int  GetQueueSize() { return queue.size(); }

    bool GetAt(int n, QImage &dest, QString &dest_data);

    void RemoveLast();

    void Clear();


//    void NewImage(const QImage& image, );

signals:

public slots:
    void NewImage(const QImage& image, const QString &data);
    void NewImageTrainner(const QImage& image);

private:

    struct QUEUE_DATA
    {
        QImage              image;
        QString             data;
    };

    QQueue< QUEUE_DATA >    queue;
    QQueue<QImage>          queue_trainner;

    int                     max_count;
};

#endif // IMAGEQUEUE_H
