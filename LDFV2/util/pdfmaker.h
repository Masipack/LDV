#ifndef PDFMAKER_H
#define PDFMAKER_H

#include <QObject>
#include <QtCore>
#include <QPainter>

class PdfMaker : public QObject
{
    Q_OBJECT
public:
    explicit PdfMaker(QObject *parent = 0);

    bool PrintPART11(QString path, QString filename, const QString &sql);

signals:

public slots:


private:
    int DrawP11Header(QPainter* painter, QRect area);
    int DrawP11Footer(QPainter* painter, QRect area, int page_num);
    int DrawP11Body(QPainter* painter, QRectF area, int start_reg_num, QVector<QRectF> &header_rects);

    int DrawRow(QPainter* painter, const QVector<QRectF> header_rects, const QFont& fnt, qreal posY,const QStringList& data);

    QString GetQuery();

    QMap<QString, QString> currentParameters;
};

#endif // PDFMAKER_H
