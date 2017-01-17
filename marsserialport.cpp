#include "marsserialport.h"

#include <QSerialPort>
#include <QFile>
#include <QDebug>
#include <QTimer>

#include "marsbytesqueue.h"


MarsSerialPort::MarsSerialPort(SerialPortSettings  stts,QObject *parent) : QSerialPort(parent)
{
    settings = stts;
    dataFrames = new MarsBytesQueue(10);
    QSerialPort::connect(this, static_cast<void(MarsSerialPort::*)(QSerialPort::SerialPortError)>(&MarsSerialPort::error),
                         this, &MarsSerialPort::handleError);
    QSerialPort::connect(this, &MarsSerialPort::readyRead,
                         this, &MarsSerialPort::decapsulate);
}

MarsSerialPort::~MarsSerialPort()
{

}

/*
 *@Desc: connect to serial port
 *@Args: SerialPortSettings
 *@Returns: None
 */
void MarsSerialPort::connect(SerialPortSettings stts)
{
    updateSettings(stts);
    connect();
}
/*
 *@Desc:connect to serial port
 *@Args:None
 *@Returns:None
 */
void MarsSerialPort::connect()
{

   setPortName(settings.name);
   setBaudRate(settings.baudRate);
   setDataBits(settings.dataBits);
   setParity(settings.parity);
   setStopBits(settings.stopBits);
   setFlowControl(settings.flowControl);
   setReadBufferSize(19);
   if(!open(QIODevice::ReadWrite))
   {
       /* serial port failed to open ,emit a signal */
       emit errors(errorInstance(tr("串口连接失败"),WARNING));
       return ;
   }
   emit connected();
}
/*
 *@Desc:close serial porta
 *@Args:None
 *@Returns:
 */

void MarsSerialPort::disconnect()
{
    /* implementation */
    if(isOpen())
    {
        close();
        /* emit a serial port closed signal to main window */
        /* wait to fuck to me*/
        emit disconnected();
    }

}

/*
 *@Desc:send byte data
 *@Args:QByteArray
 *@Returns:None
 */
void MarsSerialPort::sendByteData(QByteArray data)
{
    /* waiting you to fuck me */
    Q_UNUSED(data);
}

/*
 *@Desc:send file data
 *@Args:QFile
 *@Returns:None
 *
 */
void MarsSerialPort::sendFileData(QFile & file)
{
    /*waiting you to fuck me */
    Q_UNUSED(file);
}

/*
 *@Desc: read data from serial port and decapsulate it ,finally add it to buffer
 *@Args: None
 *@Returns: None
 *
 */
void MarsSerialPort::decapsulate()
{

    QByteArray bytes;
    static quint16 sum = 0;
    quint8 checkSum = 0;
    bytes = readAll();
    int bytesLength = bytes.length();
    for( int i = 0 ; i < bytesLength; i++)
    {
        switch((unsigned char)bytes.at(i))
        {
            case 0x55:
                readingDataFrame.clear();
                sum = 0;
                break;
            case 0xdd:
                // check data length
                if(readingDataFrame.length() != 18)
                    break;
                // check frame sum
                checkSum = (unsigned char)readingDataFrame.at(17);
                if(((sum-checkSum)%256) != checkSum)
                    break;
                // remove check sum from data
                readingDataFrame.remove(17,1);
                dataFrames->enqueue(readingDataFrame);
                readingDataFrame.clear();
                // emit dataFrameReceived signal
                emit dataFrameReceived();
                break;
            default:
                readingDataFrame.append(bytes.at(i));
                sum += (unsigned char)bytes.at(i);
                break;
        }
    }
}

/*
 *@Desc: reading data frame from frame queue
 *@Args: QByetArray & data
 *@Returns: None
 *
 */
void MarsSerialPort::readDataFrame(QByteArray &data)
{
    data = dataFrames->dequeue();
    qDebug()<<dataFrames->size();
}
/*
 *@Desc: update serial port settings
 *@Args: SerialPortSettings
 *@Returns:None
 *
 */

void MarsSerialPort::updateSettings(SerialPortSettings settings)
{
    this->settings = settings;
}

/**
 *@Desc: return error instance
 *@Args:  QString ,MarsErrorLevel
 *@Returns: MarsError
 */
MarsError MarsSerialPort::errorInstance(QString msg, MarsErrorLevel level)
{
    MarsError error;
    error.datetime = QDateTime::currentDateTime();
    error.type = CONSOLE;
    error.msg = msg;
    error.level = level;
    return error;
}

/**
 *@Desc: handle error serial port
 *@Args: SerialPort::SerialPortError error
 *@Returns: None
 */

void MarsSerialPort::handleError(QSerialPort::SerialPortError error)
{
    switch(error)
    {
        case QSerialPort::DeviceNotFoundError: //1
            emit errors(errorInstance(tr("串口错误:未发现该设备"),WARNING));
            break;
        case QSerialPort::PermissionError://2
            emit errors(errorInstance(tr("串口错误:无权限访问该设备"),WARNING));
            break;
        case  QSerialPort::OpenError://3
            emit errors(errorInstance(tr("串口错误:设备已被其他程序打开"),WARNING));
            break;
        case QSerialPort::WriteError://7
            #ifdef Q_OS_LINUX
                emit errors(errorInstance(tr("串口错误:写时错误"),WARNING));
            #endif
            break;
        case QSerialPort::ReadError://8
            #ifdef Q_OS_LINUX
                emit errors(errorInstance(tr("串口错误:读时错误"),WARNING));
            #endif
            break;
        case QSerialPort::ResourceError://9
            emit errors(errorInstance(tr("串口错误:设备不可访问"),WARNING));
            break;
        case QSerialPort::UnsupportedOperationError://10
            emit errors(errorInstance(tr("串口错误:非法操作"),WARNING));
            break;
        case QSerialPort::TimeoutError://12
            emit errors(errorInstance(tr("串口错误:设备超时"),WARNING));
            break;
        case QSerialPort::NotOpenError:// 13
            emit errors(errorInstance(tr("串口错误:设备未连接"),WARNING));
            break;
        default:
            //emit errors(errorInstance(tr("串口错误:未知错误"),WARNING));
            break;
    }
}

