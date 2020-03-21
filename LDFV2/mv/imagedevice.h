#ifndef IMAGEDEVICE_H
#define IMAGEDEVICE_H

#include <QGraphicsView>

class InspectionBuffer;

class ImageDevice : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageDevice(QWidget *parent = 0);
    virtual ~ImageDevice();

    void resetTransform();
    void Clear();

signals:
    void scaleChanged( qreal value );

public slots:
    void zoomFit();
    void zoomIn();
    void zoomOut();
    void zoomFitItems();
    void lockParent(bool v);
    void snap();

private:
    QGraphicsScene* pEmptyScene;
};

#endif // IMAGEDEVICE_H
