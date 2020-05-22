#ifndef IBPROCESS_H
#define IBPROCESS_H

#include <QObject>
#include "mv/inspectionbuffer.h"
#include "mv/tools/mvabstracttool.h"
#include "global_defines.h"
#include "TO/productto.h"
#include "util/imagequeue.h"

class IBProcess : public InspectionBuffer
{
    Q_OBJECT
public:
    explicit IBProcess(QObject *parent = 0);
    virtual ~IBProcess();

    void Reset();

    void ClearSelection();
    bool SetTO(const ProductTO &_TO);
    void GetTools(QList<MvAbstractTool*>& p) { p = tools; }
    bool GetTO(ProductTO &_TO);
    void SetOutput(const int& number_value) { number_output = number_value;}

    void SetAttributesDataBase(QMap<QString, QString> _table);
    void SetSincronizeAttributesDataBase(QMap<QString, QString> _table);

signals:
    void InspectionResult(bool bVal);

public slots:
    void NewImage(const QImage &source);
    void NewResult(bool approved, const QString& value, quint32 proc_id);

    void ClearIO();
    void SendKeepAlive();

private:
    MvAbstractTool* InsertTool(MV_TYPE type, const QString &name);
    void RemoveTool(MvAbstractTool *pItem);
    void ImageLog();

    QList<MvAbstractTool*>  tools;

    static bool ExecOrder(MvAbstractTool* s1, MvAbstractTool* s2)
    {
        return s1->GetExecutionOrder() < s2->GetExecutionOrder();
    }

    QTimer                  timerKeepAlive;
    quint32                 ok_response_count;
    quint32                 response_count;
    int                     IO_REJECT_TIME;

    bool                    RESIZE_ONE_DIRECTION;

    QImage                  img_inspection_result;

    QVector<QThread*>       threads;
    quint8                  current_thread;
    quint32                 proc_id;

    int                     number_output;

    QMap<QString, QString> table;
    QString                result_summary;

    bool                   SAVE_IMAGE;


};

#endif // IBPROCESS_H
