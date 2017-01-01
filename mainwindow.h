#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include "marstypes.h"

class QSerialPort;
class QGridLayout;
class MarsSerialPort;
class SPSettingsDialog;
class MarsCommandLine;
class MarsConsole;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    /*
     * init signals to the respective slots
     */
    void initConnections();

private slots:

     // connect to aboutAction's triggered signal
    void onAboutSoftwareActionTriggered();
    //connect to clearScreenAction's  triggered signal
    void onClearScreenActionTriggered();
    //connect  to exporeFileAction's triggered signal
    void onExportFileActionTriggered();
    // error handler
    void onApplicationError();

    /* serial port operation */
    // connect to SPSettingDialog's updated signal
    void onSerialPortSettingsUpdated(SerialPortSettings settings);
    // connect to MarsSerialPort's openFailed signal
    //void onSerialPortOpenFailed();
    // connect to MarsSerialPort's closed signal
    void onSerialPortClosed();
    // connect to MarsSerialPort's opened signal
    void onSerialPortOpened();
    // connect to MarsSerialPort's dataReady signal
    void onSerialPortDataReady();
    // connect to serialPortProtocolDialogAction's triggered signal
    void onSerialPortProtocolDialogActionTriggered();
    // connect to serialPortConfigDialogAction's triggered signal
    void onSerialPortConfigDialogActionTriggered();

    void onConsoleDataReady();

public:
    void createConsoleWindow();



private:
    Ui::MainWindow *ui;
    MarsSerialPort * devSerialPort;
    SPSettingsDialog * serialPortSettingsDialog;
    //MarsCommandLine *outConsole;
    //MarsCommandLine *inConsole;
    MarsConsole * outConsole;
    MarsConsole * inConsole;
    QGridLayout * mainWidgetGridLayout;

};

#endif // MAINWINDOW_H
