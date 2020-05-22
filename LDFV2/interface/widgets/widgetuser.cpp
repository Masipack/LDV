#include "widgetuser.h"
#include "ui_widgetuser.h"
#include "util/systemsettings.h"

/// ===========================================================================
///
/// ===========================================================================
WidgetUser::WidgetUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetUser)
{
    ui->setupUi(this);
}

/// ===========================================================================
///
/// ===========================================================================
WidgetUser::~WidgetUser()
{
    delete ui;
}

/// ===========================================================================
///
/// ===========================================================================
void WidgetUser::SetNameKey(const QString &nameKey)
{
    ui->nameKey->setText(nameKey);
    NAME_KEY = nameKey;

}

/// ===========================================================================
///
/// ===========================================================================
void WidgetUser::SetKey(const QString &key)
{
    KEY = key;
    int v;

    GetConfig(v, QString("SYSTEM/%1").arg(key), 1);

    ui->lbl_valuekey->setText(QString::number(v));
}

/// ===========================================================================
///
/// ===========================================================================
int WidgetUser::GetKeyValue()
{
    return KEY_VALUE;
}

/// ===========================================================================
///
/// ===========================================================================
void WidgetUser::SetMax(const int &v)
{
    MAX_VALUE = v;
}

/// ===========================================================================
///
/// ===========================================================================
void WidgetUser::SetMin(const int &v)
{
    MIN_VALUE = v;

}

/// ===========================================================================
///
/// ===========================================================================
void WidgetUser::WriteToFile()
{
    SystemSettings::instance()->writeReal(KEY_VALUE,QString("/SYSTEM/%1").arg(KEY));
}


/// ===========================================================================
///
/// ===========================================================================
void WidgetUser::on_btn_minus_clicked()
{
    int v = ui->lbl_valuekey->text().toInt();

    if( v <= MIN_VALUE ) return;

    v--;

    ui->lbl_valuekey->setText(QString::number(v));

    KEY_VALUE = v;
}


/// ===========================================================================
///
/// ===========================================================================
void WidgetUser::on_btn_plus_clicked()
{
    int v = ui->lbl_valuekey->text().toInt();

    if( v > MAX_VALUE) return;

    v++;

    ui->lbl_valuekey->setText(QString::number(v));

    KEY_VALUE = v;
}
