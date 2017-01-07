#include "marsserialport.h"

#include <QSerialPort>
#include <QFile>
#include <QDebug>
#include <QTimer>


MarsSerialPort::MarsSerialPort(SerialPortSettings  settings,QObject *parent) : QSerialPort(parent)
{
    this->settings = settings;
    QSerialPort::connect(this, static_cast<void(MarsSerialPort::*)(QSerialPort::SerialPortError)>(&MarsSerialPort::error),this,&MarsSerialPort::handleError);
}

MarsSerialPort::~MarsSerialPort()
{

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
}

/*
 *@Desc: recv data from serial port
 *@Args: None
 *@Returns: QByteArray
 *
 */
QByteArray MarsSerialPort::recvData()
{
    /* waiting you to fuck me */
    QByteArray data =readAll();
    return data;
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
    /*
    QTimer::singleShot(5,[=](){
        disconnect();
        connect();
    });
    */
    qDebug()<<settings.name<<"\n"
            <<settings.baudRate<<"\n"
            <<settings.stopBits<<"\n"
            <<settings.dataBits<<"\n"
            <<settings.parity<<"\n"
            <<settings.flowControl<<"\n";

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
            emit errors(errorInstance(tr("串口错误:写时错误"),WARNING));
            break;
        case QSerialPort::ReadError://8
            emit errors(errorInstance(tr("串口错误:读时错误"),WARNING));
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
