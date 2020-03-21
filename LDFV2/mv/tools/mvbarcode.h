#ifndef MVBARCODE_H
#define MVBARCODE_H

#include <QObject>
#include "mvabstracttool.h"
#include "interface/widgets/paramsbarcode.h"
#include "zxing/MultiFormatReader.h"

class MvBarcode : public MvAbstractTool
{
    Q_OBJECT
public:
    explicit MvBarcode(const QRectF &rect, MvTool *parent = nullptr);
    virtual ~MvBarcode();

    virtual bool Exec(quint32 proc_id);
    virtual void Clear();
    virtual void Reset();
    virtual bool ShowDialog(bool bEnable=true);

    void SetExpectedText(const QString& s);
    QString GetExpectedText() { return expectedText; }
    QString GetExtractedText() { return extractedText; }

    void SetPharmaCode(bool v, bool b_inv) { b_pharmacode = v; b_pharmacode_inv = b_inv; }

    typedef struct
    {
      QString type;
      QString data;
      QVector <cv::Point> location;
    } decodedObject;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
            void decode(cv::Mat &im, QVector<decodedObject> &decodedObjects);

    ParamsBarcode* form;

    zxing::MultiFormatReader *decoder;

    bool b_pharmacode;
    bool b_pharmacode_inv;

    bool PharmaCodeRead(cv::Mat& roi_local, quint32 proc_id);

    QVector<decodedObject> decodedObjects;
};


#endif // MVBARCODE_H
