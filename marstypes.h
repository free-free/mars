#ifndef MARSTYPES_H
#define MARSTYPES_H


#include <QSerialPort>
#include <QDateTime>

struct SerialPortSettings
{
    QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
};

enum MarsErrorType
{

    MAINWINDOW = 0,
    CONSOLE = 1,
    DEVSERIALPORT = 2,
};

enum MarsErrorLevel
{
    INFO = 0,
    DEBUG = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4,
};

struct MarsError
{

    MarsErrorType type;
    MarsErrorLevel  level;
    QDateTime datetime;
    QString msg;

};



#endif // MARSTYPES_H
