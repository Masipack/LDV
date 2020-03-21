#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QMutex>

class SerialControl : public QObject
{
    Q_OBJECT
public:
    enum SERIAL_COMMAND // Legacy
    {
        COMMAND_FRONT_TIME  = 0, // A
        COMMAND_FRONT_DELAY = 1, // B
        COMMAND_BACK_TIME   = 2, // C
        COMMAND_BACK_DELAY  = 3, // D
        COMMAND_BACK_SNAP   = 4, // E
        COMMAND_FRONT_SNAP  = 5, // F
        COMMAND_BACK_ON_OFF = 6, // G

        COMMAND_1116_IO_ON  = 7,
        COMMAND_1116_IO_OFF = 8,
        COMMAND_1116_BACK   = 9,
        COMMAND_1116_FRONT  = 10,
        COMMAND_1116_SNAP   = 11
    };

    inline SerialControl(QObject *parent = nullptr) { Q_UNUSED(parent); }
    ~SerialControl();

    static SerialControl *instance();

    bool init(const QString& port, quint32 baud, bool bOldFirmware=false);

    bool SetOutput( quint16 val );
    bool ToggleBitON( quint8 bitNum );
    bool ToggleBitOFF( quint8 bitNum );
    bool KeepAlive();
    bool Snap(bool bTopLight = true);
    bool SendCommand(const QString& cmd);

    // Legacy
    bool SendCommand(SERIAL_COMMAND command, int param);
    bool SendIO(quint16 value, int bit_count=15);


signals:
    void RecError(QString);
    void RecInput(QString);
    void RecValueOutputs(QString);

public slots:
    void NewData();


private:
    QSerialPort     serial;
    quint16         ioPort;

    QString _port;
    quint32 _baud;
    bool    _bOldFirmware;

    QMutex          mutex;

    QString buffer;

};

#endif // SERIALCONTROL_H
