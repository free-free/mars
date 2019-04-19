/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 * @file: marsconsole.cpp
 * @description: this file contains the implementation of console class
 * @version: 0.1
 * @author: infinite.ft
 * @date: 2017/02/07
 * @update_at: 2017/03/21
 *
 */
#include "marsconsole.h"
#include "marscommandline.h"
#include "marsbyteslistbuffer.h"
#include <QTextStream>
#include <QFileDialog>
#include <QGridLayout>
#include <QDebug>
#include <QList>
#include <QToolBar>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


/**
 *
 *@Desc: create layout, set layout styles and call initToolBar and initCmdLine method
 *@Args: QWidget *(parent ), bool(view only mode), int (max input buffer size), int(max output buffer size)
 *@Returns: None
 *
 */
MarsConsole::MarsConsole(QWidget *parent,bool viewOnly,int maxIBufferSize,int maxOBufferSize) : QWidget(parent)
{    


    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);
    // note!!!: initToolBar() must be called before createCommandLine(*);
    initToolBar();
    initCmdLine(viewOnly,maxIBufferSize,maxOBufferSize);
    connect(this,&MarsConsole::getImportFileName,this,&MarsConsole::readFile);
    connect(this,&MarsConsole::getExportFileName,this,&MarsConsole::writeFile);
}


/**
 *
 *@Desc: delete created command line instance and layout
 *@Args: None
 *@Returns: None
 *
 */
MarsConsole::~MarsConsole()
{
    for(int i=0;i<cmdLineContainer->size();++i)
    {
        delete (cmdLineContainer->at(i));
    }
    delete cmdLineNameListBox;
    delete cmdLineLayout;
    delete layout;
}



/**
 *
 *@Desc: init tool bar. creating tool bar instance and respective actions,adding action to tool bar,
 *     connecting slots to signals
 *@Args: None
 *@Returns: None
 *
 */
void
MarsConsole::initToolBar()
{
    toolBar = new QToolBar("hello",this);
    toolBar->setStyleSheet(
                           "margin:0px 0px 0px 0px;"
                           "padding:0px 0px 0px 0px;"
                           "background-color:#666;"
                           "border-left:1px solid #333;"
                           "spacing:5px;"

                           );
    toolBar->setAutoFillBackground(true);
    toolBar->setFixedHeight(30);
    toolBar->setContentsMargins(0,0,0,0);
    toolBar->setOrientation(Qt::Horizontal);
    toolBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    exportDataAction = new QAction(QIcon(":/icon/export"),tr("导出数据"),this);
    importDataAction = new QAction(QIcon(":/icon/import"),tr("导入数据"),this);
    clearCmdLineAction = new QAction(QIcon(":/icon/clear"),tr("清空输出"),this);
    plotDataAction = new QAction(QIcon(":/icon/plot"),tr("曲线图"),this);
    cmdLineNameListBox =new QComboBox(this);
    cmdLineNameListBox->setStyleSheet("QComboBox{"
                             "padding:0px 10px 0px 10px;"
                             "margin:0px 0px 0px 0px;"
                             "border:0px 0px 0px 0px;"
                             "min-width: 6em;"
                             "color:#efefef;"
                         "}"
                         "QComboBox::drop-down {"
                             "subcontrol-origin: padding;"
                             "subcontrol-position: top right;"
                             "width:20px;"
                             "border-left-width: 0px;"
                             "}"
                         "QComboBox::down-arrow {"
                            "image: url(:/icon/tiny-down-arrow);"
                         "}"
                         "QComboBox::hover{"
                            "background-color:#888;"
                         "}"
                         );
    toolBar->addSeparator();
    toolBar->addWidget(cmdLineNameListBox);
    toolBar->addAction(exportDataAction);
    toolBar->addAction(importDataAction);
    toolBar->addAction(clearCmdLineAction);
    toolBar->addAction(plotDataAction);
    connect(exportDataAction,&QAction::triggered,this,&MarsConsole::showExportDataDialog);
    connect(importDataAction,&QAction::triggered,this,&MarsConsole::showImportDataDialog);
    connect(cmdLineNameListBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCurrentCmdLine(int)));
    connect(clearCmdLineAction,&QAction::triggered,this,&MarsConsole::clearCurrentCmdLine);
    connect(plotDataAction,&QAction::triggered,this,&MarsConsole::onPlotDataActionTriggered);
    layout->addWidget(toolBar);
}


/**
 * @brief: MarsConsole::initCmdLine
 * @param: viewOnly
 * @param: maxIBufferSize
 * @param: maxOBufferSize
 * @returns: None
 */
void
MarsConsole::initCmdLine(bool viewOnly,int maxIBufferSize,int maxOBufferSize)
{
    cmdLineLayout = new QGridLayout();
    layout->addLayout(cmdLineLayout);
    cmdLineContainer = new QList<MarsCommandLine*>();
    currentCmdLine = createCmdLine(viewOnly,maxIBufferSize,maxOBufferSize);
}


/**
 * @brief: create command line,when creating successfully return created command line instance ,
 *      NULL otherwise.
 * @param: viewOnly
 * @param: maxIBufferSize, command line max input buffer size
 * @param: maxOBufferSize,command line max output buffer size
 * @returns: MarsCommandLine *
 *
 */
MarsCommandLine*
MarsConsole::createCmdLine(bool viewOnly,int maxIBufferSize, int maxOBufferSize)
{

    MarsCommandLine * tmpCmdLine;
    if(cmdLineContainer->size()>=MAX_CMDLINE_NUMBER)
       return NULL;
    tmpCmdLine = new MarsCommandLine(this,viewOnly, maxIBufferSize, maxOBufferSize);
    connect(tmpCmdLine,&MarsCommandLine::dataExportRequest,this,&MarsConsole::showExportDataDialog);
    connect(tmpCmdLine,&MarsCommandLine::dataImportRequest,this,&MarsConsole::showImportDataDialog);
    connect(tmpCmdLine,&MarsCommandLine::dataIn,this,&MarsConsole::onCmdLineDataReady);
    connect(tmpCmdLine,&MarsCommandLine::focusIn,this,&MarsConsole::changeCurrentCmdLineOnFocusIn);
    cmdLineContainer->append(tmpCmdLine);
    currentCmdLine = tmpCmdLine;
    arrangeCommandLine();
    updateStatusBar();
    return tmpCmdLine;
}


/**
 * @brief: delete current command line
 * @param: None
 * @returns: None
 */
void
MarsConsole::deleteCurrentCmdLine()
{
    deleteCmdLine(cmdLineContainer->indexOf(currentCmdLine));
}


/**
 *
 * @brief: delete command line with specific id
 * @param: index, the indice of command line in cmdLineContainer
 * @returns: None;
 *
 */
void
MarsConsole::deleteCmdLine(int index)
{
    MarsCommandLine * tmpCmdLine;
    int currentCmdLineIndex = 0;
    if(cmdLineContainer->length()<=1)
        return ;
    tmpCmdLine=cmdLineContainer->at(index);
    currentCmdLineIndex = cmdLineContainer->indexOf(currentCmdLine);
    cmdLineContainer->removeAt(index);
    delete tmpCmdLine;
    if(index == currentCmdLineIndex)
        currentCmdLine = cmdLineContainer->last();
    arrangeCommandLine();
    updateStatusBar();

}


/**
 *
 * @brief: update status bar when creating or deleting command line
 * @param: None
 * @returns: NOne
 *
 */
void
MarsConsole::updateStatusBar()
{
    disconnect(cmdLineNameListBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCurrentCmdLine(int)));
    cmdLineNameListBox->clear();
    for(int i=0;i<cmdLineContainer->length();i++)
    {
        cmdLineNameListBox->addItem("command line "+QString::number(i+1),i);
    }
    cmdLineNameListBox->setCurrentIndex(cmdLineContainer->indexOf(currentCmdLine));
    connect(cmdLineNameListBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCurrentCmdLine(int)));
    if(currentCmdLine->isReadOnly())
        importDataAction->setEnabled(false);
    else
        importDataAction->setEnabled(true);
}


/**
 *
 * @brief: MarsConsole::changeCurrentCmdLine
 * @param: index
 * @returns: None
 *
 */
void
MarsConsole::changeCurrentCmdLine(int index)
{
    currentCmdLine = cmdLineContainer->at(index);
    if(currentCmdLine->isReadOnly())
        importDataAction->setEnabled(false);
    else
        importDataAction->setEnabled(true);

}


/**
 *
 * @brief: clear current command line buffer and screen
 * @param: None
 * @returns: None
 *
 */
void
MarsConsole::clearCurrentCmdLine()
{
    currentCmdLine->clearAll();
}


/**
 *
 * @brief: slot method for MarsCommandLine's focusIn signal
 * @param: focusInObj
 * @returns: None
 */
void
MarsConsole::changeCurrentCmdLineOnFocusIn(MarsCommandLine * focusInObj)
{
    int index = cmdLineContainer->indexOf(focusInObj);
    cmdLineNameListBox->setCurrentIndex(index);
}


/**
 *
 * @brief: slot method for plotDataAction triggered signal,
 *          actually emiting plotDataRequest signal
 * @param: None
 * @returns: None
 *
 */
void
MarsConsole::onPlotDataActionTriggered()
{
     emit plotDataRequest(currentCmdLine);
}


/**
 *
 * @brief: layout command line in console window according to number of command line
 * @param: None
 * @returns: None
 *
 */
void
MarsConsole::arrangeCommandLine()
{
    switch(cmdLineContainer->size())
    {
    case 1:
        cmdLineLayout->addWidget(cmdLineContainer->at(0),1,1);
        break;
    case 2:
        cmdLineLayout->addWidget(cmdLineContainer->at(0),1,1);
        cmdLineLayout->addWidget(cmdLineContainer->at(1),1,2);
        commandLine(0)->style()->addStyleSheet("border-right:1px solid #666;");
        commandLine(1)->style()->addStyleSheet("border-left:1px solid #666;");
        break;
    case 3:
        cmdLineLayout->addWidget(cmdLineContainer->at(0),1,1,1,2);
        cmdLineLayout->addWidget(cmdLineContainer->at(1),2,1);
        cmdLineLayout->addWidget(cmdLineContainer->at(2),2,2);
        commandLine(0)->style()->addStyleSheet("border-right:1px solid #666;border-bottom:1px solid #666;");
        commandLine(1)->style()->addStyleSheet("border-left:1px solid #666;border-bottom:1px solid #666;");
        commandLine(2)->style()->addStyleSheet("border-top:1px solid #666;");
        break;
    case 4:
        cmdLineLayout->addWidget(cmdLineContainer->at(0),1,1);
        cmdLineLayout->addWidget(cmdLineContainer->at(1),1,2);
        cmdLineLayout->addWidget(cmdLineContainer->at(2),2,1);
        cmdLineLayout->addWidget(cmdLineContainer->at(3),2,2);
        commandLine(0)->style()->addStyleSheet("border-right:1px solid #666;border-bottom:1px solid #666;");
        commandLine(1)->style()->addStyleSheet("border-left:1px solid #666;border-bottom:1px solid #666;");
        commandLine(2)->style()->addStyleSheet("border-right:1px solid #666;border-top:1px solid #666;");
        commandLine(3)->style()->addStyleSheet("border-left:1px solid #666;border-top:1px solid #666;");
        break;
    }
}


/**
 *
 * @brief: resolving the specific command line instance's pointer
 * @param: index, command line instance index
 * @returns: MarsCommandLine &
 *
 */
MarsCommandLine *
MarsConsole::commandLine(int index)
{
    if(cmdLineContainer->size()==0)
        return NULL;
    if(cmdLineContainer->size()<(index-1))
        return cmdLineContainer->last();
    return cmdLineContainer->at(index);
}


/**
 *
 * @brief: slot method for command line's dataReady signal,
 *    emit dataReady signal inside
 * @param: None
 * @returns: None
 *
 */
void
MarsConsole::onCmdLineDataReady()
{
    emit dataReady();
}


/**
 *
 * @brief: show exportation data dialog
 * @param: None
 * @returns: None
 *
 */
void
MarsConsole::showExportDataDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("保存为....."),QString(),
          tr("*.txt files(*.txt);;*.json files(*.json);;*.dat files(*.dat);;"));
    if(fileName.isEmpty())
    {
        emit errors(errorInstance(tr("文件名不能为空"),WARNING));
        return ;
    }
    QTimer::singleShot(10,[=](){
        emit getExportFileName(fileName);
    });

}


/**
 *
 * @brief:  write console output buffer's data into file
 * @param: fileName
 * @returns: None
 *
 */
void
MarsConsole::writeFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QString shortFileName = (fileName.split(QRegExp("[/\\]+"))).last();
        emit errors(errorInstance(shortFileName+"打开失败",WARNING));
        return ;
    }
    QString fileType =fileName.split('.').last();
    fileType = fileType.toLower();
    try{

        if(fileType=="json")
        {
            writeJSONFile(&file);
        }
        else if(fileType =="dat")
        {
            writeDatFile(&file);
        }
        else
        {
            writeTextFile(&file);
        }
        file.close();
    }catch(exception e)
    {
        file.close();
    }
}


/**
 *
 * @brief: show importation data dialog
 * @param: None
 * @returns: None
 *
 */
void
MarsConsole::showImportDataDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("导入文件"),QString(),
            tr("*.txt files(*.txt);;*.json files(*.json);; *.dat files(*.dat);;"));
    if(fileName.isEmpty())
    {
        emit errors(errorInstance(tr("文件名不能为空"),WARNING));
        return ;
    }
    QTimer::singleShot(5,[=](){
       emit getImportFileName(fileName);
    });
}


/**
 *
 * @brief: reading data from file and import data into console input buffer
 * @param: fileName
 * @returns: None
 *
 */

void
MarsConsole::readFile(QString fileName)
{
    QFile file(fileName);
    QString shortFileName = (fileName.split(QRegExp("[/\\]+"))).last();
    if(!file.open(QIODevice::ReadOnly))
    {
        emit errors(errorInstance(shortFileName+"打开失败",WARNING));
        return ;
    }
    // max file size 20M
    if(file.size()>1024*1024*20)
    {
        emit errors(errorInstance(shortFileName+"大小已超过20M",WARNING));
        return ;
    }
    QString fileType =shortFileName.split('.').last();
    fileType = fileType.toLower();
    try{

        if(fileType=="json")
        {
            readJSONFile(&file);
        }
        else if(fileType == "dat")
        {
            readDatFile(&file);
        }
        else
        {
            readTextFile(&file);
        }
        file.close();
    }catch(exception e)
    {
        file.close();
    }
}


/**
 *
 * @brief: read text file and import data into command line input buffer
 * @param: file, the pointer of file
 * @returns: None
 *
 */
void
MarsConsole::readTextFile(QFile * file)
{
    QTextStream stream(file);
    *currentCmdLine<<stream;
}


/**
 *
 * @brief: write output buffer's data into text file
 * @param: file, the pointer of file
 * @returns: None
 *
 */
void
MarsConsole::writeTextFile(QFile * file)
{
    QTextStream stream(file);
    int dataSize = currentCmdLine->outputBuffer()->length();
    QStringList dataList;
    /* gernerating xaxis value and cascading x axis value and y axis value
     *    with one space character as seperator.
     */
    for(int i =0;i<dataSize;i++)
    {
        dataList.append(QString::number(i)+' '+currentCmdLine->outputBuffer()->at(i));
    }
    stream<<dataList.join('\n');
}


/**
 *
 * @brief: read json file and import data into  input buffer
 * @param: file, the pointer of file
 * @returns: None
 *
 */
void
MarsConsole::readJSONFile(QFile *file)
{
    QByteArray saveData=file->readAll();
    /* load data from json file */
    QJsonDocument document=QJsonDocument::fromJson(saveData);
    QJsonArray xArray=document.object().take("x").toArray();
    QJsonArray yArray=document.object().take("y").toArray();
    int dataSize = xArray.size();
    int dataColumnNum = yArray.size();
    QList<QJsonArray> yArrayList;
    QByteArray  input;
    /* check y axis value */
    for(int i=0;i<dataColumnNum;++i)
    {
        yArrayList.append(yArray.at(i).toArray());
    }
    /* convert json array to qbytearray */
    for(int i=0;i<dataSize;i++)
    {
        input.append(xArray.at(i).toVariant().toByteArray());
        for(int j =0;j<dataColumnNum;j++)
        {
            input.append(' '+yArrayList.at(j).at(i).toVariant().toByteArray());
        }
        input.append('\n');
    }
    *currentCmdLine<<input;
}


/**
 *
 * @brief: write output buffer's data into json file
 * @param: file, the pointer of file
 * @returns: None
 *
 */
void
MarsConsole::writeJSONFile(QFile * file)
{
    if(currentCmdLine->outputBuffer()->isEmpty())
        return ;
    qint64 startTime=QDateTime::currentMSecsSinceEpoch();
    QJsonObject data;
    QJsonArray yValue;
    QVariantList xValueList;
    QList<QVariantList*> yValueList;
    int dataSize = currentCmdLine->outputBuffer()->length();
    int dataColumnNum = currentCmdLine->outputBuffer()->at(0).split(' ').length();
    for(int i =0;i<dataColumnNum;i++)
    {
        yValueList.append(new QVariantList);
    }
    QList<QByteArray> dataColumnArray;
    for(int i =0 ;i<dataSize;i++)
    {
        xValueList.append(i);
        dataColumnArray = currentCmdLine->outputBuffer()->at(i).split(' ');
        for(int columnIndex =0;columnIndex<dataColumnNum;columnIndex++)
        {
           yValueList.at(columnIndex)->append(dataColumnArray.at(columnIndex).toDouble());
        }
    }
    /* convert y axis value to VariantList */
    for(int i =0;i<dataColumnNum;i++)
    {
        yValue.append(QJsonArray::fromVariantList(*(yValueList.at(i))));
        delete yValueList.at(i);
    }
    /* convert x axis value variant list to json array*/
    data["x"]=QJsonArray::fromVariantList(xValueList);
    data["y"]=yValue;
    /* writing json document to file */
    QJsonDocument saveDocument(data);
    file->write(saveDocument.toJson());
    qDebug()<<"save data time(ms): "<<QDateTime::currentMSecsSinceEpoch()-startTime;
}


/**
 *
 * @brief: read dat file and import data into  input buffer
 * @param: file, the pointer of file
 * @returns: None
 *
 */
void
MarsConsole::readDatFile(QFile *file)
{
    QByteArray saveData=file->readAll();
    /* load data from json file */
    QJsonDocument document=QJsonDocument::fromBinaryData(saveData);
    QJsonArray xArray=document.object().take("x").toArray();
    QJsonArray yArray=document.object().take("y").toArray();
    int dataSize = xArray.size();
    int dataColumnNum = yArray.size();
    QList<QJsonArray> yArrayList;
    QByteArray  input;
    /* check y axis value */
    for(int i=0;i<dataColumnNum;++i)
    {
        yArrayList.append(yArray.at(i).toArray());
    }
    /* convert json array to qbytearray */
    for(int i=0;i<dataSize;i++)
    {
        input.append(xArray.at(i).toVariant().toByteArray());
        for(int j =0;j<dataColumnNum;j++)
        {
            input.append(' '+yArrayList.at(j).at(i).toVariant().toByteArray());
        }
        input.append('\n');
    }
    *currentCmdLine<<input;

}


/**
 *
 * @brief: write output buffer's data into dat file
 * @param: file, the pointer of file
 * @returns: None
 *
 */
void
MarsConsole::writeDatFile(QFile *file)
{
    if(currentCmdLine->outputBuffer()->isEmpty())
        return ;
    qint64 startTime=QDateTime::currentMSecsSinceEpoch();
    QJsonObject data;
    QJsonArray yValue;
    QVariantList xValueList;
    QList<QVariantList*> yValueList;
    int dataSize = currentCmdLine->outputBuffer()->length();
    int dataColumnNum = currentCmdLine->outputBuffer()->at(0).split(' ').length();
    for(int i =0;i<dataColumnNum;i++)
    {
        yValueList.append(new QVariantList);
    }
    QList<QByteArray> dataColumnArray;
    for(int i =0 ;i<dataSize;i++)
    {
        xValueList.append(i);
        dataColumnArray = currentCmdLine->outputBuffer()->at(i).split(' ');
        for(int columnIndex =0;columnIndex<dataColumnNum;columnIndex++)
        {
           yValueList.at(columnIndex)->append(dataColumnArray.at(columnIndex).toDouble());
        }
    }
    /* convert y axis value to VariantList */
    for(int i =0;i<dataColumnNum;i++)
    {
        yValue.append(QJsonArray::fromVariantList(*(yValueList.at(i))));
        delete yValueList.at(i);
    }
    /* convert x axis value variant list to json array*/
    data["x"]=QJsonArray::fromVariantList(xValueList);
    data["y"]=yValue;
    /* writing json document to file */
    QJsonDocument saveDocument(data);
    file->write(saveDocument.toBinaryData());
    qDebug()<<"save data time(ms): "<<QDateTime::currentMSecsSinceEpoch()-startTime;
}


/**
 *
 * @brief: create a MarsError instance and fill it ,finally return it
 * @param:  QString ,MarsErrorLevel
 * @returns: MarsError
 *
 */
MarsError
MarsConsole::errorInstance(QString msg, MarsErrorLevel level)
{
    MarsError error;
    error.datetime = QDateTime::currentDateTime();
    error.type = CONSOLE;
    error.msg = msg;
    error.level = level;
    return error;
}
