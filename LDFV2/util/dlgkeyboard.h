#ifndef DLGKEYBOARD_H
#define DLGKEYBOARD_H

#include <QDialog>

namespace Ui {
class DlgKeyboard;
}

class DlgKeyboard : public QDialog
{
    Q_OBJECT

public:
    explicit DlgKeyboard(QWidget *parent = 0);
    ~DlgKeyboard();

    QString getText();

    void setNumbersOnly( int max_len );

    void setPasswordMode( bool bEnable );


public slots:
    void keyPressed();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_btn_caps_toggled(bool checked);

protected:
    void showEvent(QShowEvent * event);

private:
    Ui::DlgKeyboard *ui;

    int numbers_only_max_len;

    QVector<QPushButton*> bt_chars;
    QVector<QPushButton*> bt_numbers;
};

#endif // DLGKEYBOARD_H
