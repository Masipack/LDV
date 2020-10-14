#ifndef MVSAVEIMAGE_H
#define MVSAVEIMAGE_H

#include <QObject>
#include <QGraphicsScene>
#include <QtCore>

class MvSaveImage:public QObject
{
  Q_OBJECT

public:
    explicit MvSaveImage(QObject *parent=nullptr);
    virtual ~MvSaveImage();

    void SetListErros(QString& erros);
public slots:
    void Exec(QGraphicsScene * SceneCurrent,QImage image);
private:
     QString                result_summary;
     QImage                 img_inspection_result;

};

#endif // MVSAVEIMAGE_H
