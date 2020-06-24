#ifndef INSPECTIONBUFFER_H
#define INSPECTIONBUFFER_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <QGraphicsView>

class InspectionBuffer : public QGraphicsScene
{
    Q_OBJECT
public:
    enum RUN_MODE { Interactive, Processing };

    explicit InspectionBuffer(QObject *parent = 0);

    virtual void addItem(QGraphicsObject *item);

    bool ToolImage(const QRectF &roi_rect , cv::Mat &mat_dest, bool deepCopy = false);
    QImage ToolImage( const QRectF &roi_rect );

    virtual void NewImage(const QImage &source);

    virtual void Reset() = 0;

    void SetRunMode( RUN_MODE st ) { runMode = st; }
    RUN_MODE GetRunMode() { return runMode; }

    QImage GetCurrentImage() { return currentImage; }

    void FitInView();

    void FitInView(QGraphicsView *pView);

signals:

public slots:
    void lockParent(bool v);

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual void drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget = 0);

    QImage      currentImage;
    cv::Mat     mat;
    RUN_MODE    runMode;

};

#endif // INSPECTIONBUFFER_H
