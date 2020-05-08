#ifndef MVFIDUCIAL_H
#define MVFIDUCIAL_H

#include <QObject>
#include "mvabstracttool.h"
#include "interface/widgets/paramsfiducial.h"

class MvFiducial : public MvAbstractTool
{
    Q_OBJECT
public:
    explicit MvFiducial(const QRectF &rect, MvTool *parent = nullptr);
    virtual ~MvFiducial();

    virtual bool Exec(quint32 proc_id);
    virtual void Clear();
    virtual void Reset();
    virtual bool ShowDialog(bool bEnable=true);

    QPointF GetDistance();

    void SetMinimumSimilarity(int n);
    void SetMinimumRegion(int n);
    int GetMinSimilarity() { return (int)(min_correlation / 0.01); }

    void LearnCurrentTemplate();

    bool GetImageTemplate(QByteArray& dest, QSize &sz_dest);
    bool SetImageTemplate(const QByteArray &src, QSize sz);
    void ResetMove();
signals:
    void TemplateImage(const QPixmap& img);


protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double correlation(cv::Mat& image_1, cv::Mat& image_2);

    ParamsFiducial*     form;

    bool                b_approved;
    QPointF             templatePos;
    QRectF              samplingRect;

    double              correlation_val;
    double              min_correlation;
    double              min_per_region;
    double              min_per_region_old;

    cv::Mat             img_template;

    bool                bMove;
};

#endif // MVFIDUCIAL_H
