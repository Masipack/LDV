#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QtGui>
#include <QLabel>
#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

class MainDialog;

class WindowManager : public QObject
{
    Q_OBJECT
public:
    enum SCREEN_ORIENTATION { SCREEN_HORIZONTAL, SCREEN_VERTICAL };

    inline WindowManager(QObject *parent = 0) { Q_UNUSED(parent); lbl_top = 0; lbl_bottom = 0; }
    ~WindowManager();

    static WindowManager *instance();

    bool init();
    bool LoadScreens();

    SCREEN_ORIENTATION GetScreenOrientation() { return screen_orientation; }

    void SetInfoLabels(QLabel* top, QLabel* bottom) { lbl_top = top; lbl_bottom = bottom; }
    void SetUserLabels(QLabel* user, QLabel* level) { lbl_user = user; lbl_level = level; }

    void SetInfoTop(const QString& info);
    void SetInfoBottom(const QString& info);

    bool ShowScreen(const QString& window_name);
    bool ShowLastScreen();

    QWidget* GetScreen(const QString& window_name);

    void SetCurrentUser(const QString& login, int level);
    int GetCurrentUserLevel() { return current_user_level; }
    QString GetCurrentUser() { return current_user_login; }

signals:

public slots:
    void LogOff();


private:
    SCREEN_ORIENTATION          screen_orientation;

    QMap<QString, QWidget*>     v_screens;
    QMap<QString, QWidget*>     h_screens;

    MainDialog*                 mainDialog;
    QWidget*                    mainWidget;
    QWidget*                    current_screen;
    QStack<QWidget*>            last_screen;


    QLabel*                     lbl_top;
    QLabel*                     lbl_bottom;
    QLabel*                     lbl_user;
    QLabel*                     lbl_level;

    QString                     current_user_login;
    int                         current_user_level;
    QStringList                 user_level_names;

    QTimer                      logoff_timer;
    int                         logoff_minute_count;

protected:
    bool eventFilter(QObject* object, QEvent* event);

};


#endif // WINDOWMANAGER_H
