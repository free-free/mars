/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: mars.cpp
 *@description: this file contains the Mars class definition
 *@version: 0.1
 *@author: jell
 *@date: 2017/02/07
 *
 *
 */


#include "mars.h"
#include "ui_mars.h"
#include "spsettingsdialog.h"
#include "marsserialport.h"
#include "marscommandline.h"
#include "marsconsole.h"
#include "marsfigure.h"
#include "marsbyteslistbuffer.h"
#include "qcustomplot.h"
#include "helper.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QAction>
#include <QDebug>
#include <QGridLayout>
#include <QTimer>
#include <QDataStream>
#include <QMessageBox>
#include <QErrorMessage>


Mars::Mars(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mars)
{
    ui->setupUi(this);
    mainWidgetLayout = new QGridLayout(ui->mainWidget);
    mainWidgetLayout->setSpacing(0);
    mainWidgetLayout->setContentsMargins(0,0,0,0);
    serialPortSettingsDialog = new SPSettingsDialog(this);
    serialPort = new MarsSerialPort(serialPortSettingsDialog->settings(),this);
    ui->mainWidget->setLayout(mainWidgetLayout);
    console = NULL;
    figure = NULL;
    switchWindow(0);
    initConnections();
    onSerialPortClosed();
    setWindowIcon(QIcon(":/icon/mars"));
    setWindowTitle(tr("Mars"));
    setWindowIconText(tr("Mars"));
}


Mars::~Mars()
{
    delete ui;
}


/*
 *@Desc:show software information dialog
 *@Args:None
 *@Returns:None
 */
void Mars::showAboutSoftwareDialog()
{
    /*no implementation */
}


/*
 *@Desc: init Mars all signals to the respective slots
 *@Args:None
 *@Returns:None
 */
void Mars::initConnections()
{
    connect(ui->connectSPAction, &QAction::triggered,
            this, &Mars::connectSerialPort);
    connect(ui->disconnectSPAction, &QAction::triggered,
            this, &Mars::disconnectSerialPort);
    connect(ui->connectBtn, &QPushButton::clicked,
            this, &Mars::onConnectBtnClicked);
    connect(serialPort, &MarsSerialPort::disconnected,
            this, &Mars::onSerialPortClosed);
    connect(serialPort, &MarsSerialPort::connected,
            this, &Mars::onSerialPortOpened);
    connect(serialPort, &MarsSerialPort::dataFrameReceived,
            this, &Mars::renderSerialPortData);
    connect(serialPort, &MarsSerialPort::errors,
            this, &Mars::handleError);
    connect(ui->serialPortConfigDialogAction, &QAction::triggered,
            this, &Mars::showSerialPortSettingsDialog);
    connect(ui->exitAppAction, &QAction::triggered,
            this, &QApplication::exit);
    connect(ui->consoleWindowBtn, &QPushButton::clicked,
            this, &Mars::showConsoleWindow);
    connect(ui->figureWindowBtn, &QPushButton::clicked,
            this, &Mars::showFigureWindow);
}


/*
 *@Desc: connect to serial port
 *@Args: None
 *@Returns: None
 */
void Mars::connectSerialPort()
{
    serialPort->connect(serialPortSettingsDialog->settings());
}


/*
 *@Desc: disconnect from serial port
 *@Args: None
 *@Returns: None
 */
void Mars::disconnectSerialPort()
{
    serialPort->disconnect();
}


/*
 *@Desc:show serial port connection protocol dialog
 *@Args:None
 *@Returns:None
 */
void Mars::showSerialPortProtocolDialog()
{
    /* no implementation */
}


/*
 *@Desc:show serial port  config dialog
 *@Args:None
 *@Returns:None
 */
void Mars::showSerialPortSettingsDialog()
{
    serialPortSettingsDialog->exec();
}


/*
 *@Desc: slot method for connectBtn clicked,connect to serial port or disconnect serial port
 *@Args: None
 *@Returns: None
 */
void Mars::onConnectBtnClicked()
{
    if(serialPort->isOpen())
    {
        disconnectSerialPort();
    }
    else
    {
        connectSerialPort();
    }
}


/*
 *@Desc: handler MarsSerialPort's connected signal
 *@Args: None
 *@Returns: None
 */
void Mars::onSerialPortOpened()
{
    ui->connectSPAction->setEnabled(false);
    ui->serialPortConfigDialogAction->setEnabled(false);
    ui->disconnectSPAction->setEnabled(true);
    ui->connectBtn->setIcon(QIcon(":/icon/earth-connected"));
    ui->connectBtn->setToolTip(tr("关闭串口"));
}


/*
 *@Desc: handler MarsSerialPort's disconnected signal
 *@Args: None
 *@Returns: None
 */
void Mars::onSerialPortClosed()
{
    ui->serialPortConfigDialogAction->setEnabled(true);
    ui->connectSPAction->setEnabled(true);
    ui->disconnectSPAction->setEnabled(false);
    ui->connectBtn->setIcon(QIcon(":/icon/earth-disconnected"));
    ui->connectBtn->setToolTip(tr("连接串口"));
}


/*
 *@Desc: handler MarsSerialPort's dataReady signal,
 *     read serial port data and send it to figure or console
 *@Args: None
 *@Returns: None
 */
void Mars::renderSerialPortData()
{
    QByteArray data;
    QString consoleStr;
    // read data frame from serial frame queue
    serialPort->readDataFrame(data);
    if(data.isEmpty())
        return ;
    switch(currentWindowId)
    {
        case 0:
            // parse serial port data and send data to console
            for (int i = 0 ; i < 4; i++)
            {
                consoleStr.append(QString::number(bytesToFloat(data.mid(1+i*4 ,4))));
                consoleStr.append(' ');
            }
            *console->commandLine(1)<<consoleStr;
            break;
        case 1:
            // parse serial port data and send data to figure
            for (int i = 0 ; i < 4; i++)
            {
                if(data.mid(1+i*4, 4).toHex()==QByteArray(0x00000000))
                    break;
                figure->plot(bytesToFloat(data.mid(1+i*4 ,4)), i, (unsigned char)data.at(0));
            }
            break;
        default:
            break;
    }
}


/*
 *@Desc: show console window
 *@Args: None
 *@Returns: None
 */
void Mars::showConsoleWindow()
{
    if(currentWindowId != 0)
    {
        switchWindow(0);
    }
}


/*
 *@Desc: show figure window;
 *@Args: None
 *@Returns: None
 */
void Mars::showFigureWindow()
{
    if(currentWindowId != 1)
    {
        switchWindow(1);
    }
}


/*
 *@Desc: be responsible for sending console's data to serial port
 *@Args: None
 *@Returns: None
 */
void Mars::sendConsoleData()
{
    *(console->commandLine(0))>>*(console->commandLine(1));
}


/*
 *@Desc: be responsible for parse console's data and plot it's graph in figure window
 *@Args: MarsCommandLine * requestedObj
 *@Returns: None
 */
void Mars::plotConsoleData(MarsCommandLine *requestedObj)
{
    /* switch current window to figure window */
    switchWindow(1);
    /* get data size from console's data buffer */
    int dataSize = requestedObj->outputBuffer()->length();
    if(dataSize == 0)
        return ;
    QVector<double> x;
    QList<QVector<double>*> y;
    QList<QByteArray> dataLineItems;
    /* calculate graph number */
    int dataGraphNum = requestedObj->outputBuffer()->at(0).split(' ').length();
    /* create container for y axis value  */
    for(int i =0;i<dataGraphNum;i++)
    {
        y.append(new QVector<double>());
    }
    /* parse data from console's command line data */
    for(int i = 0;i<dataSize;i++)
    {
        x.append(i);
        dataLineItems = requestedObj->outputBuffer()->at(i).split(' ');
        for(int j =0;j<dataGraphNum;j++)
        {
            y.at(j)->append(dataLineItems.at(j).toDouble());
        }
    }
    int graphId = 0;
    int plotId = 0;
    /* starting plot */
    figure->startPlot();
    for(int i=0;i<dataGraphNum;i++)
    {
        /* calculate plot id , max plot number is  4 */
        plotId = ceil(i/4);
        /* calculate graph id ,max graph number of each plot is also 4 */
        graphId = i%4;
        /* starting plot */
        figure->plot(x,*y.at(i),graphId,plotId);
        delete y.at(i);
    }

}


/*
 *@Desc: create console window and connect signals to slots ,finally render it in top level
 *@Args: None
 *@Returns: None
 */
void Mars::renderConsoleWindow()
{
    if(!console)
    {
        //create MarsConsole instance
        console = new MarsConsole(ui->mainWidget,false,100000,100000);
        console->createCmdLine(true,100000,100000);
        tick = new QTimer(this);
        tick->setInterval(10);
        tick->start();
        // bing slots method to the related signal
        connect(tick,&QTimer::timeout,this,&Mars::tickTask);
        connect(console->commandLine(0),&MarsCommandLine::dataIn,this,&Mars::sendConsoleData);
        connect(console,&MarsConsole::errors,this,&Mars::handleError);
        connect(console,&MarsConsole::plotDataRequest,this,&Mars::plotConsoleData);
    }
    console->setHidden(false);
    mainWidgetLayout->addWidget(console,1,1);
}


/*
 *@Desc: create figure window and connect signals to slots ,finally render it in top level
 *@Args: None
 *@Returns: None
 */
void Mars::renderFigureWindow()
{
    if(!figure)
    {
        // create MarsFigure instance
        figure = new MarsFigure(ui->mainWidget);
        // bind slots method to the related signal
        connect(figure,&MarsFigure::error,this,&Mars::handleError);
        connect(ui->saveGraphAction,&QAction::triggered,figure,&MarsFigure::saveGraph);
    }
    figure->setHidden(false);
    mainWidgetLayout->addWidget(figure,1,1);
}


/*
 *@Desc: update menu bar's action  and widget status when current window has changed
 *@Args: int winId(0 denotes console window ,1  denotes figure window)
 *@Returns: None
 */
void Mars::updateMenuBar(int winId)
{
    switch(winId)
    {
        case 0:
            ui->saveGraphAction->setEnabled(false);
            ui->clearScreenAction->setText(tr("清空输出"));
            connect(ui->exportAction,&QAction::triggered,console,&MarsConsole::showExportDataDialog);
            connect(ui->importAction,&QAction::triggered,console,&MarsConsole::showImportDataDialog);
            connect(ui->clearScreenAction,&QAction::triggered,console,&MarsConsole::clearCurrentCmdLine);
            if(figure)
            {
                disconnect(ui->clearScreenAction,&QAction::triggered,figure,&MarsFigure::clearCurrentPlot);
                disconnect(ui->importAction,&QAction::triggered,figure,&MarsFigure::showImportDataDialog);
                disconnect(ui->exportAction,&QAction::triggered,figure,&MarsFigure::showExportDataDialog);
            }
            break;
        case 1:
            ui->saveGraphAction->setEnabled(true);
            ui->clearScreenAction->setText(tr("清除图像"));
            connect(ui->importAction,&QAction::triggered,figure,&MarsFigure::showImportDataDialog);
            connect(ui->exportAction,&QAction::triggered,figure,&MarsFigure::showExportDataDialog);
            connect(ui->clearScreenAction,&QAction::triggered,figure,&MarsFigure::clearCurrentPlot);
            if(console)
            {
                disconnect(ui->clearScreenAction,&QAction::triggered,console,&MarsConsole::clearCurrentCmdLine);
                disconnect(ui->exportAction,&QAction::triggered,console,&MarsConsole::showExportDataDialog);
                disconnect(ui->importAction,&QAction::triggered,console,&MarsConsole::showImportDataDialog);
            }
            break;
        default:
            break;
    }
}


/*
 *@Desc: remove current window and hide current window
 *@Args: None
 *@Returns: None
 */
void Mars::beforeSwitchWindow()
{

    switch(currentWindowId)
    {
        /* remove console window from layout and hide it */
        case 0:
            if(!console)
                return ;
            mainWidgetLayout->removeWidget(console);
            console->setHidden(true);
            break;
        /* remove figure window from layout and hide it*/
        case 1:
            if(!figure)
                return ;
            mainWidgetLayout->removeWidget(figure);
            figure->setHidden(true);
            break;
    }
}


/*
 *@Desc: change current window,update other related widgets
 *@Args: int windowId
 *@Returns: None
 */
void Mars::switchWindow(int windowId)
{

    beforeSwitchWindow();
    switch(windowId)
    {
        /* render figure window */
        case 1:
            renderFigureWindow();
            currentWindowId = 1;
            ui->figureWindowBtn->setDefault(true);
            ui->consoleWindowBtn->setDefault(false);
            break;
        /* render consonle window */
        default:
            renderConsoleWindow();
            currentWindowId = 0;
            ui->consoleWindowBtn->setDefault(true);
            ui->figureWindowBtn->setDefault(false);
            break;
    }
    updateMenuBar(currentWindowId);
}


/*
 *@Desc: error handler for all QWidget object
 *@Args: waiting to define
 *@Returns:None
 */
void Mars::handleError(MarsError  error)
{
    if(error.level==INFO)
    {
        QMessageBox::information(this, tr("information"),error.msg);
    }
    else if(error.level==DEBUG)
    {

    }
    else if(error.level==WARNING)
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("warning"),error.msg, 0, this);
        msgBox.setDetailedText(error.msg);
        msgBox.addButton(tr("&Continue"), QMessageBox::RejectRole);
        if (msgBox.exec() == QMessageBox::RejectRole)
             return;
    }
    else if(error.level==ERROR)
    {
        QErrorMessage errorMsg(this);
        errorMsg.showMessage(error.msg);
        errorMsg.exec();
    }
    else
    {
       QMessageBox::StandardButton reply;
       reply = QMessageBox::critical(this, tr("critical"),
             error.msg,QMessageBox::Abort | QMessageBox::Ignore);
       if (reply == QMessageBox::Abort)
           QApplication::instance()->quit();
    }
}
