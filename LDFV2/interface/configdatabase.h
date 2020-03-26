#ifndef CONFIGDATABASE_H
#define CONFIGDATABASE_H

#include <QWidget>

namespace Ui {
class ConfigDataBase;
}

class ConfigDataBase : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigDataBase(QWidget *parent = 0);
    ~ConfigDataBase();

private slots:
    void on_btn_save_clicked();

    void on_btn_cancel_clicked();

protected:
    bool Notify(QObject* object);
    void showEvent(QShowEvent * event);
    bool eventFilter(QObject* object, QEvent* event);
    void Clear();

private:
    Ui::ConfigDataBase *ui;
};

#endif // CONFIGDATABASE_H
