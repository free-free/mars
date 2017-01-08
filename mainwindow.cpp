#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spsettingsdialog.h"
#include "marsserialport.h"
#include "marscommandline.h"
#include "marsconsole.h"
#include "marsfigure.h"
#include "marsbyteslistbuffer.h"
#include "qcustomplot.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QAction>
#include <QDebug>
#include <QGridLayout>
#include <QTimer>
#include <QDataStream>
#include <QMessageBox>
#include <QErrorMessage>

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
    console = NULL;
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
    connect(ui->connectBtn,&QPushButton::clicked,this,&MainWindow::onConnectBtnClicked);
    connect(devSerialPort,&MarsSerialPort::disconnected,this,&MainWindow::onSerialPortClosed);
    connect(devSerialPort,&MarsSerialPort::connected,this,&MainWindow::onSerialPortOpened);
    connect(devSerialPort,&MarsSerialPort::readyRead,this,&MainWindow::onSerialPortDataReady);
    connect(devSerialPort,&MarsSerialPort::errors,this,&MainWindow::onApplicationError);

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
void MainWindow::onApplicationError(MarsError  error)
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
 *@Desc: slot method for connectBtn clicked,connect to serial port or disconnect serial port
 *@Args: None
 *@Returns: None
 */
void MainWindow::onConnectBtnClicked()
{
    if(devSerialPort->isOpen())
    {
        devSerialPort->disconnect();
    }
    else
    {
        devSerialPort->connect();
    }
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
    ui->connectBtn->setIcon(QIcon(":/icon/earth-connected"));
    ui->connectBtn->setToolTip(tr("关闭串口"));
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
    ui->connectBtn->setIcon(QIcon(":/icon/earth-disconnected"));
    ui->connectBtn->setToolTip(tr("连接串口"));
}

/*
 *@Desc: handler MarsSerialPort's dataReady signal,
 *     read serial port data and send it to figure or console
 *@Args: None
 *@Returns: None
 */
void MainWindow::onSerialPortDataReady()
{
    switch(currentWindowId)
    {
        /* read serial port data and send data to console */
        case 0:
            break;
        /* read serial port data and send data to figure */
        case 1:
            break;
        default:
            break;

    }
}

/*
 *@Desc: slot for consoleWindowBtn's clicked signal, switch current window to console window
 *@Args: None
 *@Returns: None
 */
void MainWindow::onConsoleWindowBtnClicked()
{
    if(currentWindowId != 0)
    {
        switchWindow(0);
    }
}

/*
 *@Desc: slot for figureWindowBtn's clicked signal, switch current window to figure window
 *@Args: None
 *@Returns: None
 */
void MainWindow::onFigureWindowBtnClicked()
{
    if(currentWindowId != 1)
    {
        switchWindow(1);
    }
}

/*
 *@Desc: slot for console's dataReady signal,
 *   be responsible for sending console's data to serial port
 *@Args: None
 *@Returns: None
 */
void MainWindow::onConsoleDataReady()
{
    *(console->commandLine(0))>>*(console->commandLine(1));
}

/*
 *@Desc: slot for console's plotDataRequest signal,
 *   be responsible for parse console's data and plot it's graph in figure window
 *@Args: MarsCommandLine * requestedObj
 *@Returns: None
 */
void MainWindow::onConsolePlotDataRequest(MarsCommandLine *requestedObj)
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
void MainWindow::renderConsoleWindow()
{

    if(!console)
    {
        console = new MarsConsole(ui->mainWidget,false,100000,100000);
        console->createCmdLine(true,100000,100000);
        tick = new QTimer(this);
        tick->setInterval(1);
        tick->start();
        connect(tick,&QTimer::timeout,this,&MainWindow::tickTask);
        connect(console->commandLine(0),&MarsCommandLine::dataIn,this,&MainWindow::onConsoleDataReady);
        connect(console,&MarsConsole::errors,this,&MainWindow::onApplicationError);
        connect(console,&MarsConsole::plotDataRequest,this,&MainWindow::onConsolePlotDataRequest);
    }
    console->setHidden(false);
    mainWidgetLayout->addWidget(console,1,1);


}

/*
 *@Desc: create figure window and connect signals to slots ,finally render it in top level
 *@Args: None
 *@Returns: None
 */
void MainWindow::renderFigureWindow()
{

    if(!figure)
    {
        figure = new MarsFigure(ui->mainWidget);
        connect(figure,&MarsFigure::error,this,&MainWindow::onApplicationError);
        connect(ui->saveGraphAction,&QAction::triggered,figure,&MarsFigure::saveGraph);
    }
    figure->setHidden(false);
    mainWidgetLayout->addWidget(figure,1,1);

}





void MainWindow::tickTask()
{
    static qint64 lastX=0;
    double x,y1,y2,y3,y4,y5,y6;
    if(lastX>=10000)
        return;
     QByteArray bytes;
      x = lastX;
      y1 = qExp(-x/150.0)*qCos(x/10.0)*10;
      y2 = qCos(x/10.0)*10;
      y3 = qExp(-x/150.0)*10;
      y4 = qSin(x/20.0)*10;
      y5 = qSin(x/30.0)*10;
      y6 = qCos(x/50.0)*10;
      bytes.append(QString::number(y1)+' ');
      bytes.append(QString::number(y2)+' ');
      bytes.append(QString::number(y3)+' ');
      bytes.append(QString::number(y4)+' ');
      bytes.append(QString::number(y5)+' ');
      bytes.append(QString::number(y6));
      *(console->commandLine(0))<<bytes;
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
/*
 *@Desc: update menu bar's action  and widget status when current window has changed
 *@Args: int winId(0 denotes console window ,1  denotes figure window)
 *@Returns: None
 */
void MainWindow::updateMenuBar(int winId)
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
void MainWindow::beforeSwitchWindow()
{

    switch(currentWindowId)
    {
        /* remove console window from layout and hide it */
        case 0:
            mainWidgetLayout->removeWidget(console);
            console->setHidden(true);
            break;
        /* remove figure window from layout and hide it*/
        case 1:
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
void MainWindow::switchWindow(int windowId)
{

    if(currentWindowId == windowId)
        return ;
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





