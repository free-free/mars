#ifndef MARSSERIALPORT_H
#define MARSSERIALPORT_H

#include <QObject>
#include "marstypes.h"


class QSerialPort;
class QFile;

class MarsSerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit MarsSerialPort(SerialPortSettings settings,QObject *parent = 0);
    ~MarsSerialPort();
    MarsError  errorInstance(QString msg, MarsErrorLevel level);

signals:
    // emit signal when serial port failed to open
    //void connectFailed(MarsError error);
    // emit signal when serial port is closed successfully
    void disconnected();
    // emit signal when serial port open successfully
    void connected();

    void errors(MarsError  error);


public slots:
    // connect to MainWindow conectSPAction's triggered signal
    void connect();
    // connect to MainWindow disconnectSPACtion's triggered signal
    void disconnect();
    // send data
    int recvRawData(QByteArray &data);
    // send raw hex data
    void sendByteData(QByteArray data);
    // send file data
    void sendFileData(QFile &file);
    // update serial port settings
    void updateSettings(SerialPortSettings settings);
private:
    void handleError(QSerialPort::SerialPortError error);
private:

    SerialPortSettings  settings;
};

#endif // MarsSerialPort_H
