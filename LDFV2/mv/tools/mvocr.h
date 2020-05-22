#ifndef MVOCR_H
#define MVOCR_H

#include <QObject>
#include "mvabstracttool.h"
#include "interface/widgets/paramsocr.h"
#include "mv/PO/mvocrpo.h"

class MvOCR : public MvAbstractTool
{
    Q_OBJECT
public:
    explicit MvOCR(const QRectF &rect, MvTool *parent = nullptr);
    virtual ~MvOCR();

    virtual bool Exec(quint32 proc_id);
    virtual void Clear();
    virtual void Reset();
    virtual bool ShowDialog(bool bEnable=true);


    void SetExpectedText(const QString& s);
    QString GetExpectedText()       { return expectedText; }
    QString GetExtractedText()      { return extractedText; }

    void SetWhiteFilterSize(int n);
    int  GetWhiteFilterSize()       { return white_filter; }
    void SetBlackAndWhite(const int n);
    int  GetBlackAndWhite();
    QObject* GetPO() { return (QObject*)&PO; }

    void SetRotationStateTool(const bool& v);

    void ResetMove();

public slots:
    void ExecResult(const QImage& img, const QString& text, quint32 prid);
    void NewAttribute(const QString value);

signals:
    void NewResultAttributes(const QString &value);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double correlation(cv::Mat& image_1, cv::Mat& image_2);

    ParamsOCR*      form;
    MvOCRPO         PO;

    int  white_filter;
    bool b_approved;
    bool b_Busy;
    int  b_black_And_white;


};

#endif // MVOCR_H
