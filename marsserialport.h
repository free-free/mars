#ifndef MARSSERIALPORT_H
#define MARSSERIALPORT_H

#include <QObject>
#include "marstypes.h"


class QSerialPort;
class QFile;
class MarsBytesQueue;

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
    // signal for serial port recevied one data frame
    void dataFrameReceived();


public slots:
    // connect to serial port
    void connect();
    void connect(SerialPortSettings stts);
    // diconnect serial port
    void disconnect();

    // send raw hex data
    void sendByteData(QByteArray data);
    // send file data
    void sendFileData(QFile &file);
    // update serial port settings
    void updateSettings(SerialPortSettings settings);
    // read data frame from frame buffer queue
    void readDataFrame(QByteArray &data);

private:
    void handleError(QSerialPort::SerialPortError error);
    // decapsulate data, add decapsulated data to buffer
    void decapsulate();
private:

    SerialPortSettings  settings;
    QByteArray readingDataFrame;
    MarsBytesQueue * dataFrames;

};

#endif // MarsSerialPort_H
