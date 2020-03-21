#ifndef MVABSTRACTTOOL_H
#define MVABSTRACTTOOL_H

#include "mv/mvtool.h"
#include <QLayout>
#include "global_defines.h"

class MvAbstractTool : public MvTool
{
    Q_OBJECT
public:
    explicit MvAbstractTool(const QRectF &rect, QGraphicsItem *parent = 0);
    virtual ~MvAbstractTool();

    virtual bool Exec() { return Exec(0); }
    virtual bool Exec(quint32 proc_id)                  = 0;
    virtual void Clear()                                = 0;
    virtual bool ShowDialog(bool bEnable=true)          = 0;
    virtual void Reset()                                = 0;
    void SetLayout(QLayout* p) { pLayout = p; }


    void SetToolName(const QString& name);

    void SetType(MV_TYPE t)         { mv_type = t; }
    MV_TYPE GetType()               { return mv_type; }
    QPointF GetConfigPos()          { return configPos; }
    int GetExecutionOrder()         { return executionOrder; }
    void SetExecutionOrder(int n)   { executionOrder = n; }
    void ReconfigPosition();
    bool CompareResults()           { return (expectedText == extractedText); }

    void ShowThresholded(bool v)    { bShowThreshold = v; }

    QString GetExpectedText()       { return expectedText; }

    void SetConfigPos(QPointF p )   { configPos = p; }

signals:
    void NameChanged(const QString& name);
    void NewResult(bool approved, const QString& value, quint32 proc_id);

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

protected:
    QLayout*        pLayout;

    MV_TYPE         mv_type;

    QPointF         configPos;

    int             executionOrder;

    QString         expectedText;

    QString         extractedText;

    bool            bShowThreshold;

    QImage          result_image;

    QElapsedTimer   tm_exec;
};

#endif // MVABSTRACTTOOL_H
