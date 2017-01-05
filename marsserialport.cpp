#include "marsserialport.h"

#include <QSerialPort>
#include <QFile>
#include <QDebug>


MarsSerialPort::MarsSerialPort(SerialPortSettings  settings,QObject *parent) : QObject(parent)
{
    this->settings = settings;

}


/*
 *@Desc:connect to serial port
 *@Args:None
 *@Returns:None
 */
void MarsSerialPort::connect()
{

   serialPort = new QSerialPort(this);
   serialPort->setPortName(settings.name);
   serialPort->setBaudRate(settings.baudRate);
   serialPort->setDataBits(settings.dataBits);
   serialPort->setParity(settings.parity);
   serialPort->setStopBits(settings.stopBits);
   serialPort->setFlowControl(settings.flowControl);
   if(!serialPort->open(QIODevice::ReadWrite))
   {
        /* serial port failed to open ,emit a signal */
        /*..........*/

       //emit connectFailed();
       return ;
   }
   QObject::connect(serialPort,&QSerialPort::readyRead,this,&MarsSerialPort::recvData);
   emit connected();
}

/*
 *@Desc:close serial port
 *@Args:None
 *@Returns:
 */

void MarsSerialPort::disconnect()
{
    /* implementation */
    if(serialPort->isOpen())
    {
        serialPort->close();
    }

    /* emit a serial port closed signal to main window */
    /* wait to fuck to me*/
    emit disconnected();
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
 *@Returns:
 *
 */
void MarsSerialPort::recvData()
{
    /* waiting you to fuck me */
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

    qDebug()<<settings.name<<"\n"
            <<settings.baudRate<<"\n"
            <<settings.stopBits<<"\n"
            <<settings.dataBits<<"\n"
            <<settings.parity<<"\n"
            <<settings.flowControl<<"\n";

}
