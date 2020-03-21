#ifndef IBCONFIG_H
#define IBCONFIG_H

#include "mv/inspectionbuffer.h"
#include "mv/tools/mvabstracttool.h"
#include "global_defines.h"
#include "TO/productto.h"

class IBConfig : public InspectionBuffer
{
    Q_OBJECT
public:
    explicit IBConfig(QObject *parent = 0);
    virtual ~IBConfig();

    void Reset();

    MvAbstractTool* InsertTool(MV_TYPE type, const QString &name);

    void RemoveTool(MvAbstractTool* pItem);
    void ExecPosUP(MvAbstractTool* pItem);
    void ExecPosDN(MvAbstractTool* pItem);

    void ClearSelection();

    bool GetTO(ProductTO &_TO);

signals:

public slots:
    void NewImage(const QImage &source);

private:
    QList<MvAbstractTool*>  tools;

    static bool ExecOrder(MvAbstractTool* s1, MvAbstractTool* s2)
    {
        return s1->GetExecutionOrder() < s2->GetExecutionOrder();
    }
};

#endif // IBCONFIG_H
