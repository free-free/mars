#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spsettingsdialog.h"
#include "marsserialport.h"
#include "marscommandline.h"
#include "marsconsole.h"
#include "marsfigure.h"
#include "qcustomplot.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QAction>
#include <QDebug>
#include <QGridLayout>
#include <QTimer>
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWidgetLayout = new QGridLayout(ui->mainWidget);
    mainWidgetLayout->setSpacing(0);
    mainWidgetLayout->setContentsMargins(0,0,0,0);
    serialPortSettingsDialog = new SPSettingsDialog(this);
    devSerialPort = new MarsSerialPort(serialPortSettingsDialog->settings(),this);
    ui->mainWidget->setLayout(mainWidgetLayout);
    inConsole=NULL;
    outConsole=NULL;
    figure = NULL;
    renderConsoleWindow();
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

    connect(ui->consoleWindowBtn,&QPushButton::clicked,this,&MainWindow::onConsoleWindowBtnClicked);
    connect(ui->figureWindowBtn,&QPushButton::clicked,this, &MainWindow::onFigureWindowBtnClicked);
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

void MainWindow::onConsoleWindowBtnClicked()
{
    if(currentWindowId != 0)
    {
        switchWindow(0);
        ui->consoleWindowBtn->setDefault(true);
        ui->figureWindowBtn->setDefault(false);
    }
}

void MainWindow::onFigureWindowBtnClicked()
{
    if(currentWindowId != 1)
    {
        switchWindow(1);
        ui->figureWindowBtn->setDefault(true);
        ui->consoleWindowBtn->setDefault(false);
    }
}



void MainWindow::renderConsoleWindow()
{
    if(!outConsole)
        outConsole = new MarsConsole(ui->mainWidget,true);
    if(!inConsole)
        inConsole = new MarsConsole(ui->mainWidget);
    inConsole->commandLine(0).style()->addStyleSheet("border-right:1px solid #666;");
    outConsole->commandLine(0).style()->addStyleSheet("border-left:1px solid #666;");
    inConsole->setHidden(false);
    outConsole->setHidden(false);
    mainWidgetLayout->addWidget(inConsole,1,1);
    mainWidgetLayout->addWidget(outConsole,1,2);
    connect(inConsole,&MarsConsole::dataReady,this,&MainWindow::onConsoleDataReady);
    currentWindowId = 0;

}


void MainWindow::renderFigureWindow()
{

    if(!figure)
    {
        figure = new MarsFigure(ui->mainWidget);
        tick = new QTimer(this);
        tick->setInterval(1000);
        tick->start();
        connect(tick,&QTimer::timeout,this,&MainWindow::tickTask);
    }
    figure->setHidden(false);
    mainWidgetLayout->addWidget(figure,1,1);
    currentWindowId = 1;

}



void MainWindow::onConsoleDataReady()
{
    (inConsole->commandLine(0))>>(outConsole->commandLine(0));
}

void MainWindow::tickTask()
{
    static int lastX=0;
    double x,y1,y2;

      x = lastX;
      y1 = qExp(-x/150.0)*qCos(x/10.0)*10;
      y1 = qCos(x/10.0)*5;
      y2 = qExp(-x/150.0)*5;
    //qDebug()<<"x:"<<x<<"y1:"<<y1<<"y2:"<<y2;
     //figure->plot(x,y1,0,0);
     //figure->plot(x,y2,1,1);
    //QByteArray data="1,2,3;1,3,4;1,3,4\n2,3,4;2,4,5;2,4,5\n3,4,5;3,5,6;3,4,5\n4,3,4;4,4,2;4,3,2";
      //inStream<<12.3243;
      //inStream<<21.32;
      //QDataStream outStream(data);
      //outStream>>y;
      //qDebug()<<data;
      //qDebug()<<"hello";
      //qDebug()<<y;
   // figure->plot(data);
    lastX++;

}

void MainWindow::beforeSwitchWindow()
{

    switch(currentWindowId)
    {
        // remove console window from layout and hide it
        case 0:
            mainWidgetLayout->removeWidget(inConsole);
            mainWidgetLayout->removeWidget(outConsole);
            inConsole->setHidden(true);
            outConsole->setHidden(true);
            break;
        // remove figure window from layout and hide it
        case 1:
            mainWidgetLayout->removeWidget(figure);
            figure->setHidden(true);
            break;
    }
}


void MainWindow::switchWindow(int windowId)
{

    if(currentWindowId == windowId)
        return ;
    beforeSwitchWindow();
    switch(windowId)
    {
        // render console window
        case 0:
            renderConsoleWindow();
            break;
        // render figure window
        case 1:
            renderFigureWindow();
            break;
        default:
            renderConsoleWindow();
            break;
    }
}





