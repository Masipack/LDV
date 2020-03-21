#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

#include <QtCore>

enum MV_TYPE
{
    MV_FIDUCIAL = 0x01, MV_OCR = 0x02, MV_BARCODE = 0x03, MV_DATAMATRIX = 0x04, MV_TYPE_UNDEFINED = 0xFF
};

struct DATABASE
{
    DATABASE() {}

    QString  url;
    QString  name;
    QString  user;
    QString  password;
    QString  port;
    QString  table;

    bool IsNull(){
       return  url.isNull()||name.isNull() || user.isNull() || password.isNull() || port.isNull() || table.isNull();
    }
     bool IsEmpty(){
        return url.isEmpty()||name.isEmpty() || user.isEmpty() || password.isEmpty() || port.isEmpty() || table.isEmpty();
    }
};



class Result
{
public:
     Result() {}
     bool value;
     QString error ;
};


//----------------------------------------------------------------
#define Debug(text)  qDebug() << Q_FUNC_INFO << __LINE__ << text;
#define DISP2(a,b) { namedWindow(a, WINDOW_NORMAL); imshow(a, b); }

#endif // GLOBAL_DEFINES_H
