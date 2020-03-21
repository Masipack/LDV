#include "serialcontrol.h"

#include <QDebug>
#include <QtCore>
#include "util/alarmmanager.h"

Q_GLOBAL_STATIC(SerialControl, serialControl)

/// ===========================================================================
///
/// ===========================================================================
SerialControl *SerialControl::instance()
{
    return serialControl();
}

/// ===========================================================================
///
/// ===========================================================================
SerialControl::~SerialControl()
{
}

/// ===========================================================================
///
/// ===========================================================================
bool SerialControl::init(const QString& port, quint32 baud, bool bOldFirmware)
{
    _port           = port;
    _baud           = baud;
    _bOldFirmware   = bOldFirmware;

    if( serial.isOpen() )
    {
        serial.close();
    }

    serial.setPortName( port );
    serial.setBaudRate( baud );
    serial.setDataBits( QSerialPort::Data8 );
    serial.setStopBits( QSerialPort::OneStop );
    serial.setFlowControl( QSerialPort::NoFlowControl );
    serial.setParity( QSerialPort::NoParity );

    bool res = serial.open(QIODevice::ReadWrite);

    ioPort = 0;

    if( res )
    {
        QString cmd = QString("#O0X");
        if( serial.write( cmd.toLatin1().data() ) == -1 ) return false;
        serial.flush();
    }

    connect(&serial, SIGNAL(readyRead()), this, SLOT(NewData()), Qt::UniqueConnection );

    return res;
}

///============================================================================
///
///============================================================================
void SerialControl::NewData()
{
    QString s = serial.readAll();
    for(int i = 0; i < s.size(); i++ )
    {
        if( buffer.size() == 0 && s[i] != '$' ) continue;

        if( s[i] == 'X' )
        {
            if( buffer[1] == 'I' )
            {
                buffer.clear();
                emit( RecInput( buffer.mid(2,4)) );
            }

            if( buffer[1] == 'E' )
            {
                buffer.clear();
                emit( RecError( buffer.mid(2,2)) );
            }

            if( buffer[1] == 'V' )
            {
                buffer.clear();
                emit( RecValueOutputs( buffer.mid(2,6)) );
            }
        }
        else
           buffer += s[i];
    }
}

///============================================================================
///
///============================================================================
bool SerialControl::SetOutput( quint16 val )
{
    if( serial.isOpen() == false && init(_port, _baud, _bOldFirmware) == false )  return false;

    ioPort = val;
    QString cmd = QString("#O%1X").arg(val, 3, 16, QChar('0')).toUpper();
    if( serial.write( cmd.toLatin1().data() ) == -1 ) return false;
    serial.flush();
    ioPort = val;

    return true;
}

///============================================================================
///
///============================================================================
bool SerialControl::ToggleBitON( quint8 bitNum )
{
    if( serial.isOpen() == false && init(_port, _baud, _bOldFirmware) == false )  return false;
    if( bitNum > 15 )               return false;

    ioPort = ioPort | (1 << bitNum);

    QString cmd = QString("#O%1X").arg(ioPort, 4, 16, QChar('0')).toUpper();
    if( serial.write( cmd.toLatin1().data() ) == -1 ) return false;
    serial.flush();

    return true;
}

///============================================================================
///
///============================================================================
bool SerialControl::ToggleBitOFF( quint8 bitNum )
{
    if( serial.isOpen() == false && init(_port, _baud, _bOldFirmware) == false )  return false;
    if( bitNum > 15 )               return false;

    ioPort = ioPort & (0xFFFF - (1 << bitNum));

    QString cmd = QString("#O%1X").arg(ioPort, 4, 16, QChar('0')).toUpper();
    if( serial.write( cmd.toLatin1().data() ) == -1 ) return false;
    serial.flush();

    return true;

}

///============================================================================
///
///============================================================================
bool SerialControl::KeepAlive()
{
    if( serial.isOpen() == false && init(_port, _baud, _bOldFirmware) == false )  return false;
    if( serial.write( "#KX" ) == (-1) ) return false;
    serial.flush();
    return true;
}

///============================================================================
///
///============================================================================
bool SerialControl::Snap(bool bTopLight)
{
    if( serial.isOpen() == false && init(_port, _baud, _bOldFirmware) == false )  return false;

    if( _bOldFirmware )
    {
        if( bTopLight )
        {
            this->SendCommand( SerialControl::COMMAND_1116_FRONT, 1 );
            this->SendCommand( SerialControl::COMMAND_1116_BACK , 0 );
        }
        else
        {
            this->SendCommand( SerialControl::COMMAND_1116_FRONT, 0 );
            this->SendCommand( SerialControl::COMMAND_1116_BACK , 1 );
        }

        return this->SendCommand( SerialControl::COMMAND_1116_SNAP,  1 );
    }

    if( bTopLight )
    {
        if( serial.write( "#LB0X#LF1X#TX" ) == (-1) ) return false;
    }
    else
    {
        if( serial.write( "#LB1X#LF0X#TX" ) == (-1) ) return false;
    }
    serial.flush();
    return true;
}

///============================================================================
///
///============================================================================
bool SerialControl::SendCommand(const QString& cmd)
{
    if( serial.isOpen() == false && init(_port, _baud, _bOldFirmware) == false )  return false;

    if( _bOldFirmware == false )
    {
        if( serial.write( cmd.toLatin1().data() ) == (-1) ) return false;
        serial.flush();
    }
    else
    {
        return false;
    }
    return true;
}

///============================================================================
/// Legacy firmware
///============================================================================
bool SerialControl::SendCommand(SERIAL_COMMAND command, int param)
{
    if( serial.isOpen() == false && init(_port, _baud, _bOldFirmware) == false )  return false;

    if( command < COMMAND_1116_IO_ON )
    {
        QString s;
        s = QString("%1%2X").arg(QChar('A' + command)).arg(param,4, 10, QChar('0'));
        serial.write( s.toLatin1().data() );
        serial.waitForBytesWritten( s.size() * 2 );
        serial.flush();
        return true;
    }

    QString s;
    if( command == COMMAND_1116_SNAP )
    {
        s = QString("H1000X\r\n");
        serial.write( s.toLatin1().data() );
        serial.waitForBytesWritten( s.size() * 2 );
        serial.flush();
        return true;
    }

    if( command == COMMAND_1116_IO_ON )
    {
        ioPort |= (1 << param);
        if( param <= 7 )
            s = QString("S0%1X\r\n").arg(ioPort & 0xFF, 3, 10, QChar('0'));
        else
            s = QString("S1%1X\r\n").arg((ioPort & 0xFF00) >> 8, 3, 10, QChar('0'));
    }

    if( command == COMMAND_1116_IO_OFF )
    {
        ioPort &= (0xFFFF - (1 << param));
        if( param <= 7 )
            s = QString("S0%1X\r\n").arg(ioPort & 0xFF, 3, 10, QChar('0'));
        else
            s = QString("S1%1X\r\n").arg((ioPort & 0xFF00) >> 8, 3, 10, QChar('0'));
    }

    if( command == COMMAND_1116_BACK )
    {
        s = QString("U000%1X\r\n").arg(param);
    }

    if( command == COMMAND_1116_FRONT)
    {
        s = QString("U000%1X\r\n").arg(param);
    }

    serial.write( s.toLatin1().data() );
    serial.waitForBytesWritten( s.size() );
    serial.flush();
    return true;
}

///============================================================================
/// Legacy firmware
///============================================================================
bool SerialControl::SendIO(quint16 value, int bit_count)
{
    if( serial.isOpen() == false && init(_port, _baud, _bOldFirmware) == false )  return false;

    ioPort = (value & 0xFE);
    QString s = QString("S0%1X\r\n").arg(ioPort & 0xFF, 3, 10, QChar('0'));
    serial.write( s.toLatin1().data() );
    serial.waitForBytesWritten( s.size() );
    serial.flush();

    if( bit_count > 8 )
    {
        s = QString("S1%1X\r\n").arg((ioPort & 0xFF00) >> 8, 3, 10, QChar('0'));
        serial.write( s.toLatin1().data() );
        serial.waitForBytesWritten( s.size() );
        serial.flush();
    }
    return true;
}
