#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spsettingsdialog.h"
#include "marsserialport.h"
#include "marscommandline.h"
#include "marsconsole.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QAction>
#include <QDebug>
#include <QGridLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWidgetGridLayout = new QGridLayout(ui->mainWidget);
    serialPortSettingsDialog = new SPSettingsDialog(this);
    devSerialPort = new MarsSerialPort(serialPortSettingsDialog->settings(),this);
    ui->mainWidget->setLayout(mainWidgetGridLayout);
    createConsoleWindow();
    initConnections();
    onSerialPortClosed();

}

MainWindow::~MainWindow()
{
    delete ui;
}



/*
 *@Desc:show software information dialog
 *@Args:None
 *@Returns:None
 */

void MainWindow::onAboutSoftwareActionTriggered()
{
    /*.............*/
}


void MainWindow::onClearScreenActionTriggered()
{
    /*............*/
}


/*
 *@Desc: export data info file(*.jpg,*.xls,*.csv)
 *@Args:None
 *@Returns:None
 */
void MainWindow::onExportFileActionTriggered()
{
    /*...........*/
}


/*
 *@Desc: init mainwindow all signals to the respective slots
 *@Args:None
 *@Returns:None
 */
void MainWindow::initConnections()
{
    connect(ui->connectSPAction,&QAction::triggered,devSerialPort,&MarsSerialPort::connect);
    connect(ui->disconnectSPAction,&QAction::triggered,devSerialPort,&MarsSerialPort::disconnect);
    connect(devSerialPort,&MarsSerialPort::disconnected,this,&MainWindow::onSerialPortClosed);
    connect(devSerialPort,&MarsSerialPort::connected,this,&MainWindow::onSerialPortOpened);
    connect(devSerialPort,&MarsSerialPort::dataReady,this,&MainWindow::onSerialPortDataReady);
    connect(devSerialPort,&MarsSerialPort::connectFailed,this, &MainWindow::onApplicationError);
    connect(devSerialPort,&MarsSerialPort::connectFailed,this,&MainWindow::onApplicationError);

    connect(serialPortSettingsDialog,&SPSettingsDialog::updated,this,&MainWindow::onSerialPortSettingsUpdated);
    connect(ui->serialPortConfigDialogAction,&QAction::triggered,this,&MainWindow::onSerialPortConfigDialogActionTriggered);
    connect(ui->exitAppAction,&QAction::triggered,this,&QApplication::exit);


}

/*
 *@Desc:show serial port connection protocol dialog
 *@Args:None
 *@Returns:None
 */

void MainWindow::onSerialPortProtocolDialogActionTriggered()
{
    /* waiting you to fuck me */
}

/*
 *@Desc:show serial port  config dialog
 *@Args:None
 *@Returns:None
 */

void MainWindow::onSerialPortConfigDialogActionTriggered()
{
    /* waiting you to fuck me */
    serialPortSettingsDialog->exec();
}

/*
 *@Desc: error handler for all QWidget object
 *@Args: waiting to define
 *@Returns:None
 */
void MainWindow::onApplicationError()
{
    /* fuck me ,baby */
}

/*
 *@Desc: handle SPSettingsDialog's updated signal
 *@Args: SerialPortSettings
 *@Returns: None
 */

void MainWindow::onSerialPortSettingsUpdated(SerialPortSettings settings)
{
    devSerialPort->updateSettings(settings);
}

/*
 *@Desc: handler MarsSerialPort's opened signal
 *@Args: None
 *@Returns: None
 */
void MainWindow::onSerialPortOpened()
{
    ui->connectSPAction->setEnabled(false);
    ui->serialPortConfigDialogAction->setEnabled(false);
    ui->disconnectSPAction->setEnabled(true);
}

/*
 *@Desc: handler MarsSerialPort's closed signal
 *@Args: None
 *@Returns: None
 */
void MainWindow::onSerialPortClosed()
{
    ui->serialPortConfigDialogAction->setEnabled(true);
    ui->connectSPAction->setEnabled(true);
    ui->disconnectSPAction->setEnabled(false);


}


/*
 *@Desc: handler MarsSerialPort's closed signal
 *@Args: None
 *@Returns: None
 */
void MainWindow::onSerialPortDataReady()
{
    //
}


void MainWindow::createConsoleWindow()
{
    /*
    outConsole  = new MarsCommandLine(ui->mainWidget,true);
    inConsole = new MarsCommandLine(ui->mainWidget);
    inConsole->style()->addStyleSheet("border-right:1px solid #666;");
    outConsole->style()->addStyleSheet("border-left:1px solid #666;");
    outConsole->style()->prompt(">>>>>   ");
    mainWidgetGridLayout->addWidget(inConsole,1,0);
    mainWidgetGridLayout->addWidget(outConsole,1,1);
    mainWidgetGridLayout->setSpacing(0);
    mainWidgetGridLayout->setContentsMargins(0,0,0,0);
    connect(inConsole,&MarsCommandLine::dataReadyRead,this,&MainWindow::onCmdConsoleDataReady);
    */
    outConsole = new MarsConsole(ui->mainWidget,true);
    inConsole = new MarsConsole(ui->mainWidget);
    inConsole->commandLine(0).style()->addStyleSheet("border-right:1px solid #666;");
    outConsole->commandLine(0).style()->addStyleSheet("border-left:1px solid #666;");
    mainWidgetGridLayout->addWidget(inConsole,1,0);
    mainWidgetGridLayout->addWidget(outConsole,1,1);
    mainWidgetGridLayout->setSpacing(0);
    mainWidgetGridLayout->setContentsMargins(0,0,0,0);
    connect(inConsole,&MarsConsole::dataReady,this,&MainWindow::onConsoleDataReady);
}



void MainWindow::onConsoleDataReady()
{
    (inConsole->commandLine(0))>>(outConsole->commandLine(0));
}

















