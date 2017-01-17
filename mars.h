#ifndef MARS_H
#define MARS_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include "marstypes.h"

class QSerialPort;
class QGridLayout;
class MarsSerialPort;
class SPSettingsDialog;
class MarsCommandLine;
class MarsConsole;
class MarsFigure;
class QTimer;

namespace Ui {
class Mars;
}

class Mars : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mars(QWidget *parent = 0);
    ~Mars();

private:
    /*
     * init signals to the respective slots
     */
    void initConnections();

private slots:

     // connect to aboutAction's triggered signal
    void showAboutSoftwareDialog();
    // error handler
    void handleError(MarsError  error);
    /* serial port operation */
    void onSerialPortClosed();
    void onSerialPortOpened();
    void sendConsoleData();
    void showSerialPortProtocolDialog();
    void showSerialPortSettingsDialog();
    void showConsoleWindow();
    void showFigureWindow();
    void onConnectBtnClicked();
    void renderSerialPortData();
    void plotConsoleData(MarsCommandLine * requestedObj);
    void connectSerialPort();
    void disconnectSerialPort();

public:
    void renderConsoleWindow();
    void renderFigureWindow();
    void switchWindow(int windowId);
    void updateMenuBar(int winId);
    void tickTask();

private:
    void beforeSwitchWindow();

private:
    Ui::Mars *ui;
    MarsSerialPort * serialPort;
    SPSettingsDialog * serialPortSettingsDialog;
    MarsConsole * console;
    MarsFigure *  figure;
    QGridLayout * mainWidgetLayout;
    int currentWindowId;
    QTimer *tick;

};

#endif // MAINWINDOW_H
