#include "formpart11.h"
#include "ui_formpart11.h"
#include <QtCore>
#include "util/dlgkeyboard.h"
#include "interface/windowmanager.h"
#include "interface/dlgdatetime.h"
#include "util/pdfmaker.h"
#include <QScrollBar>
#include <QtSql>
#include "util/systemsettings.h"
#include "util/dlginfo.h"

/// ===========================================================================
///
/// ===========================================================================
FormPart11::FormPart11(QWidget *parent) :  QWidget(parent),  ui(new Ui::FormPart11)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->le_batch->installEventFilter(this);
    ui->le_user->installEventFilter(this);
    ui->le_equipment->installEventFilter(this);
    ui->le_dt_start->installEventFilter(this);
    ui->le_dt_end->installEventFilter(this);
}

/// ===========================================================================
///
/// ===========================================================================
FormPart11::~FormPart11()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void FormPart11::showEvent(QShowEvent * event)
{
    Q_UNUSED(event)
    WindowManager::instance()->SetInfoTop( "CFR Part11" );
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    GetConfig(strPedido, "FABRIMA/PEDIDO", QString("0000") );
}

/// ===========================================================================
///
/// ===========================================================================
bool FormPart11::eventFilter(QObject* object, QEvent* event)
{

//    ui->le_batch->installEventFilter(this);
//    ui->le_user->installEventFilter(this);
//    ui->le_equipment->installEventFilter(this);
//    ui->le_dt_start->installEventFilter(this);
//    ui->le_dt_end->installEventFilter(this);

    if(object == ui->le_batch && event->type() == QEvent::MouseButtonRelease )
    {
        ui->le_batch->setText( "" );
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_batch->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_user && event->type() == QEvent::MouseButtonRelease )
    {
        ui->le_user->setText( "" );
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_user->setText( dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_equipment && event->type() == QEvent::MouseButtonRelease )
    {
        ui->le_equipment->setText( "" );
        DlgKeyboard dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_equipment->setText( "dlg.getText()" );
        }
        return false;
    }

    if(object == ui->le_dt_start && event->type() == QEvent::MouseButtonRelease )
    {
       // ui->le_dt_start->setText( "" );

        DlgDateTime dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_dt_start->setText(dlg.getText().isNull() || dlg.getText().isEmpty() ? "01/01/2000 00:00:00": dlg.getText() );
        }
        return false;
    }

    if(object == ui->le_dt_end && event->type() == QEvent::MouseButtonRelease )
    {
       // ui->le_dt_end->setText( "" );
        DlgDateTime dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            ui->le_dt_end->setText( dlg.getText().isNull() || dlg.getText().isEmpty() ? "01/01/2000 00:00:00" : dlg.getText() );
        }
        return false;
    }

    return false;

}

/// ===========================================================================
///
/// ===========================================================================
void FormPart11::resizeEvent(QResizeEvent *event)
{
    int n = (ui->tableWidget->width() - 4 - ui->tableWidget->verticalScrollBar()->width()) / 6;
    ui->tableWidget->setColumnWidth(0, n);
    ui->tableWidget->setColumnWidth(1, n);
    ui->tableWidget->setColumnWidth(2, n);
    ui->tableWidget->setColumnWidth(3, n);
    ui->tableWidget->setColumnWidth(4, n);
    ui->tableWidget->setColumnWidth(5, n);

    QWidget::resizeEvent(event);
}

/// ===========================================================================
///
/// ===========================================================================
QString FormPart11::GetQuery()
{
    // "select * from audit where (TimeStamp > '2016-08-10 01:51:00.000' AND TimeStamp < '2016-08-10 01:53:00.000') AND Lote = '1234' AND UserID like 'sistema' and ObjectID like 'aplicação'"

    QString sql_composed;

    bool bFirstOK = false;
    if( !ui->le_batch->text().isEmpty() )
    {
        if( bFirstOK == false )
        {
            bFirstOK = true;
            sql_composed += " WHERE";
        }
        else
            sql_composed += " AND";

        sql_composed += " Lote = '" + ui->le_batch->text() + "'";
    }

    if( !ui->le_user->text().isEmpty() )
    {
        if( bFirstOK == false )
        {
            bFirstOK = true;
            sql_composed += " WHERE";
        }
        else
            sql_composed += " AND";

        sql_composed += " UserID LIKE '%" + ui->le_user->text() + "%'";
    }

    if( !ui->le_equipment->text().isEmpty() )
    {
        if( bFirstOK == false )
        {
            bFirstOK = true;
            sql_composed += " WHERE";
        }
        else
            sql_composed += " AND";

        sql_composed += " ObjectID LIKE '%" + ui->le_equipment->text() + "%'";
    }

    if( !ui->le_dt_start->text().isEmpty() && !ui->le_dt_end->text().isEmpty() )
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


        QStringList A = ui->le_dt_start->text().split(QChar(' '));
        QStringList B = ui->le_dt_end->text().split(QChar(' '));

        QString s;
        s   = QString("%1 %2").arg(A.at(0)).arg(A.at(1));
        tm1 = QDateTime::fromString(s, "dd/MM/yyyy hh:mm:ss");
        s   = QString("%1 %2").arg(B.at(0)).arg(B.at(1));
        tm2 = QDateTime::fromString(s, "dd/MM/yyyy hh:mm:ss");

        // (TimeStamp > '2016-08-10 01:51:00.000' AND TimeStamp < '2016-08-10 01:53:00.000')
        sql_composed += " (TimeStamp > '" + tm1.toString("yyyy-MM-dd hh:mm:ss.zzz");
        sql_composed += "' AND TimeStamp < '" + tm2.toString("yyyy-MM-dd hh:mm:ss.zzz") + QString("')");
    }

    return sql_composed;
}

/// ===========================================================================
///
/// ===========================================================================
void FormPart11::on_btn_menu_clicked()
{
    WindowManager::instance()->ShowLastScreen();
}

/// ===========================================================================
///
/// ===========================================================================
void FormPart11::on_btn_filter_clicked()
{

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QString s = QString("SELECT Lote, TimeStamp, UserID, ObjectID, Description, Comment from Audit ") + GetQuery() + QString(" ORDER BY TimeStamp ASC");

    QSqlQuery query(QString(), QSqlDatabase::database("PART11"));
    if( query.exec( s ) == false )
    {
        return;
    }

    while( query.next() )
    {
        ui->tableWidget->insertRow(0);
        for(int i = 0; i < 6; i++)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(query.value(i).toString());
            ui->tableWidget->setItem(0, i, newItem);
        }
    }

    int n = (ui->tableWidget->width() - 4 - ui->tableWidget->verticalScrollBar()->width()) / 6;
    ui->tableWidget->setColumnWidth(0, n);
    ui->tableWidget->setColumnWidth(1, n);
    ui->tableWidget->setColumnWidth(2, n);
    ui->tableWidget->setColumnWidth(3, n);
    ui->tableWidget->setColumnWidth(4, n);
    ui->tableWidget->setColumnWidth(5, n);
}


/// ===========================================================================
///
/// ===========================================================================
void FormPart11::on_btn_export_pdf_clicked()
{
//    QProcess p;
//    QString usr = getenv("USER"); ///for MAc or Linux
//    p.start("df -h");
//    p.waitForFinished();

//    QString usb;
//    while(p.canReadLine())
//    {
//        QString s = p.readLine();
//        if( s.contains( "/media/" + usr + "/") ) usb = s.split(' ').last();
//    }

//    usb.remove(QRegExp("[\\n\\t\\r]"));

    QString usbPath;
    QList<QStorageInfo> info = QStorageInfo::mountedVolumes();

    for( int i = 0; i < info.size(); i++ )
    {
        if( info.at(i).rootPath().contains("/media/") )
        {
            if( usbPath != (info.at(i).rootPath() + "/SVF_" + strPedido) )
            {
                usbPath = info.at(i).rootPath() + "/SVF_" + strPedido;
            }
            break;
        }
    }

    if( usbPath.isEmpty() )
    {
        DlgInfo dlg;
        dlg.SetMessage(DlgInfo::IT_INFO, tr("Insira Drive USB !"), false, false );
        dlg.exec();
        return;
    }

    if( !QDir(usbPath).exists() )
    {
        if( QDir().mkdir(usbPath) == false )
        {
            LOG(LOG_ERROR_TYPE, tr("Falha criando pasta em drive usb"));
            return;
        }
    }

    PdfMaker pdf;
    pdf.PrintPART11(usbPath + "/", QString("PART11") + QDate::currentDate().toString("_dd_MM_yyyy"), GetQuery() );
}

/// ===========================================================================
///
/// ===========================================================================
void FormPart11::on_btn_clean_clicked()
{
    ui->le_batch->setText("");
    ui->le_user->setText("");
    ui->le_equipment->setText("");
    ui->le_dt_start->setText("01/01/2000 00:00:00");
    ui->le_dt_end->setText("31/12/2100 23:59:59");
    on_btn_filter_clicked();
}
