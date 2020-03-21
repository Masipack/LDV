#ifndef PRODUCTTO_H
#define PRODUCTTO_H

#include <QtCore>
#include "TO/abstracttoolto.h"
#include "TO/fiducialto.h"
#include "TO/ocrto.h"
#include "TO/barcodeto.h"
#include "TO/datamatrixto.h"

class ProductTO
{
public:
    QString                 CAM_NAME;
    QVector<FiducialTO>     LIST_FIDUCIAL;
    QVector<OcrTO>          LIST_OCR;
    QVector<BarcodeTO>      LIST_BARCODE;
    QVector<DatamatrixTO>   LIST_DATAMATRIX;


    //NOT SERIALIZABLE:

    friend QDataStream &operator >>(QDataStream &s, ProductTO& v)
    {
        s >> v.CAM_NAME;
        s >> v.LIST_FIDUCIAL;
        s >> v.LIST_OCR;
        s >> v.LIST_BARCODE;
        s >> v.LIST_DATAMATRIX;
        return s;
    }

    friend QDataStream &operator <<(QDataStream &s, const ProductTO& v)
    {
        s << v.CAM_NAME;
        s << v.LIST_FIDUCIAL;
        s << v.LIST_OCR;
        s << v.LIST_BARCODE;
        s << v.LIST_DATAMATRIX;
        return s;
    }

    friend QDebug operator <<(QDebug dbg, const ProductTO& v)
    {
        dbg.space() << "\nProductTO:\n";
        dbg.space() << v.CAM_NAME;
        dbg.space() << v.LIST_FIDUCIAL;
        dbg.space() << v.LIST_OCR;
        dbg.space() << v.LIST_BARCODE;
        dbg.space() << v.LIST_DATAMATRIX;
        return dbg.space();
    }
};

#endif // PRODUCTTO_H
