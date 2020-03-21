#ifndef MVDATAMATRIX_H
#define MVDATAMATRIX_H

#include <QObject>
#include "mvabstracttool.h"
#include "interface/widgets/paramsdatamatrix.h"

class MvDataMatrix : public MvAbstractTool
{
    Q_OBJECT
public:
    explicit MvDataMatrix(const QRectF &rect, MvTool *parent = nullptr);
    virtual ~MvDataMatrix();

    virtual bool Exec(quint32 proc_id);
    virtual void Clear();
    virtual void Reset();
    virtual bool ShowDialog(bool bEnable=true);

    void SetExpectedText(const QString& s);
    QString GetExpectedText() { return expectedText; }
    QString GetExtractedText() { return extractedText; }

    typedef struct
    {
      QString type;
      QString data;
      QVector <cv::Point> location;
    } decodedObject;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    ParamsDataMatrix* form;

    QVector<decodedObject> decodedObjects;

    void Init();
};

#endif // MVDATAMATRIX_H
