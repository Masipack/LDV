#ifndef IBIMAGELOG_H
#define IBIMAGELOG_H

#include "mv/inspectionbuffer.h"

class IBImageLog : public InspectionBuffer
{
    Q_OBJECT
public:
    explicit IBImageLog(QObject *parent = 0);

    void Reset();

signals:

public slots:
    void NewImage(const QImage &source);

};

#endif // IBIMAGELOG_H
