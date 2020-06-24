#ifndef MVOCRCONFIG_H
#define MVOCRCONFIG_H

#include <mv/mvtool.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/ml/ml.hpp>
#include "opencv2/objdetect.hpp"
#include "mv/PO/mvocrpo.h"
#include "mvabstracttool.h"

using namespace  cv;
using namespace  std;

class MvOCRConfig : public MvAbstractTool
{
    Q_OBJECT
public:
    explicit MvOCRConfig(const QRectF &rect, MvTool *parent = nullptr);
    virtual ~MvOCRConfig();


    virtual bool Exec(quint32 proc_id);
    virtual void Clear();
    virtual void Reset();
    virtual bool ShowDialog(bool bEnable=true){;}

    bool GetItem(int index,MvOCRPO::DESCRIPTOR& dest);
    bool SetItem(int index, QChar c);

    MvOCRPO::DESCRIPTOR        GetCONTOUR(const int index);
    QVector<float> GetDescripters(int index);

    void SetCurrentItem(int i) { currentItem = i; }
    void RemoteAt(int i);

    void setBackground(quint32 v){ BlackAndWhite = v; }

signals:
    void TemplateChanged();

public slots:

protected:

    bool   ExtractedCaracterHOG(const vector< std::vector<cv::Point> >& contours, const vector< Vec4i >& hierarchy, const QPointF& TL);
    QImage GetImage();

    void AddItem(const vector<float>& descriptors,const Rect& br,const QPointF& top_left);

    void Initialize();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:

    QList<MvOCRPO::DESCRIPTOR>  contours_found;

    int currentItem;
    int THRESHOLD;
    int DISTANCE;

    int MAX_WIDTH;
    int MIN_WIDTH;
    int MAX_HEIGTH;
    int MIN_HEIGTH;

    quint32 BlackAndWhite;

    int white_filter;
    QList<int> filterSizes;
};

#endif // MVOCRCONFIG_H
