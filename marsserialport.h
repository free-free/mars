#ifndef MARSSERIALPORT_H
#define MARSSERIALPORT_H

#include <QObject>
#include "marstypes.h"


class QSerialPort;
class QFile;

class MarsSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MarsSerialPort(SerialPortSettings settings,QObject *parent = 0);

signals:
    // emit signal when serial port failed to open
    void connectFailed(QStringList errorMsg);
    // emit signal when serial port is closed successfully
    void disconnected();
    // emit signal when serial port open successfully
    void connected();
    // emit signal when recv a completely package
    void dataReady();



public slots:
    // connect to MainWindow conectSPAction's triggered signal
    void connect();
    // connect to MainWindow disconnectSPACtion's triggered signal
    void disconnect();
    // connect to QSerialPort's readyRead signal
    void recvData();

public:
    // send raw hex data
    void sendByteData(QByteArray data);
    // send file data
    void sendFileData(QFile &file);
    // update serial port settings
    void updateSettings(SerialPortSettings settings);

private:
    QSerialPort * serialPort;
    SerialPortSettings  settings;
};

#endif // MarsSerialPort_H
