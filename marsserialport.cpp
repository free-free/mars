/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 * @file: marsserialport.cpp
 * @description: this file contains the implementation of mars serial port class
 * @version: 0.1
 * @author: infinite.ft
 * @date: 2017/02/07
 * @update_at: 2017/03/21
 *
 *
 */

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


/**
 *
 * @brief: connect to serial port
 * @param: SerialPortSettings
 * @returns: None
 *
 */
void
MarsSerialPort::connect(SerialPortSettings stts)
{
    updateSettings(stts);
    connect();
}


/**
 *
 * @brief: connect to serial port
 * @param: None
 * @returns: None
 *
 */
void
MarsSerialPort::connect()
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


/**
 *
 * @brief: close serial porta
 * @param: None
 * @returns:None
 *
 */
void
MarsSerialPort::disconnect()
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


/**
 *
 * @brief: send byte data
 * @param: data
 * @returns: None
 *
 */
void
MarsSerialPort::sendByteData(QByteArray data)
{
    /* waiting you to fuck me */
    Q_UNUSED(data);
}


/**
 *
 * @brief: send file data
 * @param: file
 * @returns: None
 *
 */
void
MarsSerialPort::sendFileData(QFile & file)
{
    /*waiting you to fuck me */
    Q_UNUSED(file);
}


/**
 *
 * @brief: read data from serial port and decapsulate it ,finally add it to buffer
 * @param: None
 * @returns: None
 *
 */
void
MarsSerialPort::decapsulate()
{

    QByteArray bytes;
    quint8 checkSum = 0;
    int bytesLength = 0;
    static quint16 sum = 0;

    bytes = readAll();
    bytesLength = bytes.length();
    for( int i = 0 ; i < bytesLength; i++)
    {
        switch((quint8)bytes.at(i))
        {
                // start to receive data
            case 0x55:
                readingDataFrame.clear();
                sum = 0;
                break;
            case 0xdd:
                // check data length
                if(readingDataFrame.length() != 22)
                {
                    break;
                }
                // check frame sum
                checkSum = (quint8)readingDataFrame.at(21);
                if(((sum- checkSum) % 256) != checkSum)
                {
                    break;
                }
                // remove check sum from data
                readingDataFrame.remove(21,1);
                dataFrames->enqueue(readingDataFrame);
                readingDataFrame.clear();
                // emit dataFrameReceived signal
                emit dataFrameReceived();
                break;
            default:
                readingDataFrame.append(bytes.at(i));
                sum += (quint8)bytes.at(i);
                break;
        }
    }
}


/**
 *
 * @brief: reading data frame from frame queue
 * @param: data
 * @returns: None
 *
 */
void
MarsSerialPort::readDataFrame(QByteArray &data)
{
    data = dataFrames->dequeue();
}


/**
 *
 * @brief: update serial port settings
 * @param: settings
 * @returns:None
 *
 */
void
MarsSerialPort::updateSettings(SerialPortSettings settings)
{
    this->settings = settings;
}


/**
 *
 * @brief: return error instance
 * @param: msg, message string
 * @param: level
 * @returns: MarsError
 *
 */
MarsError
MarsSerialPort::errorInstance(QString msg, MarsErrorLevel level)
{
    MarsError error;
    error.datetime = QDateTime::currentDateTime();
    error.type = CONSOLE;
    error.msg = msg;
    error.level = level;
    return error;
}


/**
 *
 * @brief: handle error serial port
 * @param: error
 * @returns: None
 *
 */
void
MarsSerialPort::handleError(QSerialPort::SerialPortError error)
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
