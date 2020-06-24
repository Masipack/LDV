#ifndef IBCONFIGOCR_H
#define IBCONFIGOCR_H

#include "mv/inspectionbuffer.h"
#include "mv/tools/mvocrconfig.h"
#include "mv/PO/mvocrpo.h"


class IBConfigOCR : public InspectionBuffer
{
    Q_OBJECT
public:
    explicit IBConfigOCR (QObject *parent = 0);
    virtual ~IBConfigOCR();

     void Reset();

     bool    GetItem(int index, MvOCRPO::DESCRIPTOR& dest) { return ocr_cfg->GetItem(index, dest); }
     bool    SetItem(int index, QChar c) { return ocr_cfg->SetItem(index, c); }

     MvOCRPO::DESCRIPTOR GetCONTOUR(int index) { return ocr_cfg->GetCONTOUR(index); }
     QVector<float>      GetDescriptores(int index) { return ocr_cfg->GetDescripters(index); }

     void RemoveAt(int i){ocr_cfg->RemoteAt(i);}
     void SetCurrentItem(int i)   { ocr_cfg->SetCurrentItem(i); }
     void setBackground(quint32 v){ ocr_cfg->setBackground(v);
                                    ocr_cfg->Exec(0);
                                  }

     void Exec(){ ocr_cfg->Exec(0);}

signals:
     void TemplateChanged();

public slots:
    void NewImage(const QImage &source);


private:

     MvOCRConfig*     ocr_cfg;


};

#endif // IBCONFIGOCR_H
