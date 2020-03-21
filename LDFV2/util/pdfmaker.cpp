#include "pdfmaker.h"

#include <QFile>

#include "util/dlginfo.h"
#include "QPdfWriter"

#include <QTextDocument>
#include <QTextStream>
#include <QFile>
#include <QPrinter>
#include <QDir>
#include <QtSql>
#include <QTextLayout>

/// ===========================================================================
///
/// ===========================================================================
PdfMaker::PdfMaker(QObject *parent) : QObject(parent)
{
}

/// ===========================================================================
///
/// ===========================================================================
bool PdfMaker::PrintPART11(QString path, QString filename,const QString& sql)
{
    if( path.endsWith('/') == false ) path += "/";

    if( QFile::exists(path + filename) )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_QUESTION, tr("Deseja sobrescrever '") + filename + "' ?", true, true );
        dlg.exec();

        if( dlg.result() == QDialog::Rejected ) return false;
    }

    QFile pdf_file;
    pdf_file.setFileName( path + filename );
    pdf_file.open(QIODevice::WriteOnly);
    if( pdf_file.isOpen() == false )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_ERROR, tr("Erro ao gerar arquivo: ") + pdf_file.errorString() );
        dlg.exec();
        return false;
    }

    QPdfWriter pdf(&pdf_file);
    pdf.setPageLayout(QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMarginsF()));

    QPainter painter(&pdf);
    QRect draw_area = painter.viewport();
    draw_area.adjust(draw_area.width() / 20, 0, -draw_area.width() / 20, 0);
    draw_area.adjust(0, draw_area.height() / 30, 0, -draw_area.height() / 30);

    int spc = draw_area.height() * 0.01;

    const int header_height = draw_area.height() * 0.05;
    const int footer_height = draw_area.height() * 0.03;

    QRect rc_header = draw_area.adjusted(0,0,0, header_height-draw_area.height() );
    QRect rc_footer = draw_area.adjusted(0,draw_area.height() - footer_height,0,0);
    QRect rc_body   = draw_area.adjusted(0, rc_header.height() + spc,0,-(rc_footer.height() + spc));
    QVector<QRectF> header_rects;

    // ----- table
    QSqlQuery query(QString(), QSqlDatabase::database("PART11"));
    QString SQL_FILTER = sql.isEmpty() ? GetQuery() : sql;
    if( query.exec( QString("SELECT Lote, TimeStamp, UserID, ObjectID, Description, Comment from Audit ") + SQL_FILTER + QString(" ORDER BY TimeStamp ASC")) == false )
        return false;

    int pagenum = 1;

    DrawP11Header(&painter, rc_header);
    DrawP11Footer(&painter, rc_footer, pagenum++);
    DrawP11Body(&painter, QRectF(rc_body), 0, header_rects);

    qreal posY = header_rects[0].bottom();

    QFont fnt2("Courier 10 Pitch", 6);
    painter.setFont( fnt2 );

    while(query.next())
    {
        QStringList data;
        for(int i = 0; i < 6; i++)
        {
            data << query.value(i).toString();
        }

        posY += DrawRow(&painter, header_rects, fnt2, posY, data);
        data.clear();

        if( posY > QRectF(rc_body).bottom() )
        {
            pdf.newPage();
            DrawP11Header(&painter, rc_header);
            DrawP11Footer(&painter, rc_footer, pagenum++);
            DrawP11Body(&painter, QRectF(rc_body), 0, header_rects);

            posY = header_rects[0].bottom();
        }

    };



    painter.end();

    pdf_file.flush();
    pdf_file.close();

    return true;
}

/// ===========================================================================
///
/// ===========================================================================
int PdfMaker::DrawP11Body(QPainter* painter, QRectF area, int start_reg_num, QVector<QRectF>& header_rects)
{
    header_rects.clear();

    painter->setBrush( Qt::NoBrush );
    painter->setPen(Qt::black);

    QStringList txt_header;
    txt_header << tr("Lote") << tr("Data/Hora") << tr("Usuário") << tr("Equipamento") << tr("Descrição") << tr("Comentário");

    QVector<qreal> column_sizes;
    column_sizes << (4.54 / 100.0) << (15.28  / 100.0) << (13.24 / 100.0) << (30.62 / 100.0) << (50.0 / 100.0) << (50.0 / 100.0);

    QFont fnt2("Courier 10 Pitch", 6);
    painter->setFont( fnt2 );


    for(int i = 0; i < column_sizes.size(); i++)
    {
        QRectF rc = area;
        rc.setHeight( (qreal)area.height() * 0.011 );

        if( i != 0 )    rc.setLeft(header_rects[i-1].right());
        rc.setWidth( (qreal)rc.width() * column_sizes[i]  );
        if( i == column_sizes.size() -1 ) rc.setRight( area.right() );

        header_rects.push_back( rc );
        painter->drawText(rc, txt_header[i], QTextOption(Qt::AlignCenter) );
    }



    return 0;
}

/// ===========================================================================
///
/// ===========================================================================
int PdfMaker::DrawRow(QPainter* painter, const QVector<QRectF> header_rects, const QFont& fnt, qreal posY,const QStringList& data)
{
    QFontMetricsF fm(fnt, painter->device());


    int breaks[6] = {0,0,0,0,0,0};
    qreal sz[6] = {0,0,0,0,0,0};

    int max_breaks = 0;

    for(int i = 0; i < 6; i++)
    {
        if( data[i].isEmpty() ) continue;

        qreal char_sz = fm.width(data[i]);
        if( char_sz < (header_rects[i].width() * 0.95) ) continue;

        qreal b                 = char_sz / (header_rects[i].width() * 0.95);

        if( b > max_breaks )
        {
            breaks[i] = qFloor(b);
            max_breaks = breaks[i];
            qreal sobra = char_sz - max_breaks * (header_rects[i].width() * 0.95);
            qreal txt_sobra = (sobra * data[i].size()) / char_sz;
            sz[i] = (data[i].size() - qFloor(txt_sobra)) / max_breaks;
        }

    }

    int h = fm.height(); //header_rects[0].height();// * (breaks + 1);

    int rcmax = 0;

    for(int i = 0; i < 6; i++)
    {
        QString s = data[i];

        QRectF rc = header_rects[i];
        rc.moveBottom( posY + header_rects[0].height() );

        if( max_breaks > 0 )
        {
            for(int b = breaks[i]; b > 0; b--)
            {
                s.insert(b * sz[i], QString("\n") );
            }

            for(int b = max_breaks; b > 0; b--)
            {
                rc.adjust(0,0,0,h);
            }

        }

        if( i < 4 )
        {
            painter->drawText(rc, s, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter) );
        }
        else
        {
            painter->drawText(rc.adjusted( rc.width() * 0.025,0, -rc.width() * 0.025, 0 ), s, QTextOption(Qt::AlignLeft | Qt::AlignVCenter) );
        }

        painter->drawLine(rc.bottomLeft(), rc.bottomRight());

        if( i > 0 ) painter->drawLine(rc.bottomLeft() - QPointF(0, rc.height() * 0.15), rc.topLeft() + QPointF(0, rc.height() * 0.15) );

        rcmax = qMax((int)rc.height(), rcmax);
    }

    return rcmax;
}

/// ===========================================================================
///
/// ===========================================================================
int PdfMaker::DrawP11Header(QPainter* painter, QRect area)
{
    QImage logo(":/images/logo.png");
    QRect im_area = area.adjusted(0, area.height() * 0.2, 0, -area.height() * 0.2 );
    im_area.setWidth( ((qreal)logo.width() / (qreal)logo.height()) * (qreal)im_area.height() );
    painter->drawImage(im_area, QImage(":/images/logo.png") );

    QRect text_area = area;
    text_area.setLeft( im_area.right() );
    text_area.setRight( area.right() - area.width() * 0.2 );

    QFont fnt("Courier 10 Pitch", 12);
    fnt.setBold(true);
    painter->setFont( fnt );
    painter->setPen(Qt::darkGray);

    painter->drawText(text_area.adjusted(0, 10, 0, 10), tr("Relatório CFR21 - Part11"), QTextOption(Qt::AlignCenter) );

    QRect date_area = area;
    date_area.setLeft( text_area.right() );

    QFont fnt2("Courier 10 Pitch", 8);
    painter->setFont( fnt2 );

    painter->drawText(date_area, QDateTime::currentDateTime().toString("yyyy-MMM-dd\nhh:mm:ss"), QTextOption(Qt::AlignVCenter | Qt::AlignRight) );

    painter->setPen(Qt::black);
    painter->drawLine(area.bottomLeft(), area.bottomRight());

    return 0;
}

/// ===========================================================================
///
/// ===========================================================================
int PdfMaker::DrawP11Footer(QPainter* painter, QRect area, int page_num)
{
    painter->setPen(Qt::black);
    //painter->drawLine(area.topLeft(), area.topRight() );


    QFont fnt2("Courier 10 Pitch", 8);
    painter->setFont( fnt2 );

    QRect footer_area = area.adjusted(0, area.height() * 0.2, 0, -area.height() * 0.2 );
    painter->drawText(footer_area, "Pag: - " + QString::number(page_num) + " -", QTextOption(Qt::AlignCenter) );

    return 0;
}

/// ===========================================================================
///
/// ===========================================================================
QString PdfMaker::GetQuery()
{
    // "select * from audit where (TimeStamp > '2016-08-10 01:51:00.000' AND TimeStamp < '2016-08-10 01:53:00.000') AND Lote = '1234' AND UserID like 'sistema' and ObjectID like 'aplicação'"

    QString vals;
    foreach (QString s, currentParameters.values())
    {
        vals += s;
    }

    if( vals.size() == 0 )
    {
        return QString("");
    }


    QString sql_composed;
    // QMap(("DATA_1", "2017-02-01")("DATA_2", "2017-02-02")("EQUIPAMENTO", "EQUIPA")("HORA_1", "01:01")("HORA_2", "22:22")("LOTE", "LOTE")("USUARIO", "USUARIO"))


    bool bFirstOK = false;
    if( !currentParameters.value("LOTE").isEmpty() )
    {
        if( bFirstOK == false )
        {
            bFirstOK = true;
            sql_composed += " WHERE";
        }
        else
            sql_composed += " AND";

        sql_composed += " Lote = '" + currentParameters.value("LOTE") + "'";
    }

    if( !currentParameters.value("USUARIO").isEmpty() )
    {
        if( bFirstOK == false )
        {
            bFirstOK = true;
            sql_composed += " WHERE";
        }
        else
            sql_composed += " AND";

        sql_composed += " UserID LIKE '%" + currentParameters.value("USUARIO") + "%'";
    }

    if( !currentParameters.value("EQUIPAMENTO").isEmpty() )
    {
        if( bFirstOK == false )
        {
            bFirstOK = true;
            sql_composed += " WHERE";
        }
        else
            sql_composed += " AND";

        sql_composed += " ObjectID LIKE '%" + currentParameters.value("EQUIPAMENTO") + "%'";
    }

    if( !currentParameters.value("DATA_1").isEmpty() && !currentParameters.value("DATA_2").isEmpty() )
    {
        if( bFirstOK == false )
        {
            bFirstOK = true;
            sql_composed += " WHERE";
        }
        else
            sql_composed += " AND";

        QDateTime tm1;
        QDateTime tm2;
        if( !currentParameters.value("HORA_1").isEmpty() && !currentParameters.value("HORA_2").isEmpty() )
        {
            // Recebido: 2017-02-02 01:01 -> Sqlite: 2016-08-10 01:53:00.000
            QString s;
            s   = QString("%1 %2").arg(currentParameters.value("DATA_1")).arg(currentParameters.value("HORA_1"));
            tm1 = QDateTime::fromString(s, "dd/MM/yyyy hh:mm");
            s   = QString("%1 %2").arg(currentParameters.value("DATA_2")).arg(currentParameters.value("HORA_2"));
            tm2 = QDateTime::fromString(s, "dd/MM/yyyy hh:mm");
        }
        else
        {
            QString s;
            s   = QString("%1").arg(currentParameters.value("DATA_1"));
            s += " 00:00:00";
            tm1 = QDateTime::fromString(s, "dd/MM/yyyy hh:mm:ss");
            s   = QString("%1").arg(currentParameters.value("DATA_2"));
            s += " 23:59:59";
            tm2 = QDateTime::fromString(s, "dd/MM/yyyy hh:mm:ss");
        }

        // (TimeStamp > '2016-08-10 01:51:00.000' AND TimeStamp < '2016-08-10 01:53:00.000')
        sql_composed += " (TimeStamp > '" + tm1.toString("yyyy-MM-dd hh:mm:ss.zzz");
        sql_composed += "' AND TimeStamp < '" + tm2.toString("yyyy-MM-dd hh:mm:ss.zzz") + QString("')");
    }

    return sql_composed;
}
