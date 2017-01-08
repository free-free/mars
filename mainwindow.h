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
class MarsFigure;
class QTimer;

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
    void onApplicationError(MarsError  error);

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

    void onConsoleWindowBtnClicked();
    void onFigureWindowBtnClicked();
    void onConnectBtnClicked();

    void onConsoleDataReady();
    void onConsolePlotDataRequest(MarsCommandLine * requestedObj);

public:
    void renderConsoleWindow();
    void renderFigureWindow();
    void switchWindow(int windowId);
    void updateMenuBar(int winId);
    void tickTask();
private:

    void beforeSwitchWindow();

private:
    Ui::MainWindow *ui;
    MarsSerialPort * devSerialPort;
    SPSettingsDialog * serialPortSettingsDialog;
    MarsConsole * console;
    MarsFigure *  figure;
    QGridLayout * mainWidgetLayout;
    int currentWindowId;
    QTimer *tick;

};

#endif // MAINWINDOW_H
