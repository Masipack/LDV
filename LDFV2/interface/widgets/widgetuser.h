#ifndef WIDGETUSER_H
#define WIDGETUSER_H

#include <QWidget>

namespace Ui {
class WidgetUser;
}

class WidgetUser : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetUser(QWidget *parent = 0);
    ~WidgetUser();

    void SetNameKey(const QString &nameKey);
    void SetKey(const QString& key);
    int  GetKeyValue();
    void SetMax(const int& v);
    void SetMin(const int& v);
    void WriteToFile();



private slots:
    void on_btn_minus_clicked();

    void on_btn_plus_clicked();

private:
    Ui::WidgetUser *ui;

    QString       KEY;
    QString       NAME_KEY;
    int           KEY_VALUE;
    int           MIN_VALUE;
    int           MAX_VALUE;
};

#endif // WIDGETUSER_H
