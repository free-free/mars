#include "marsconsole.h"
#include "marscommandline.h"
#include <QTextStream>
#include <QFileDialog>
#include <QGridLayout>
#include <QDebug>
#include <QList>
#include <QToolBar>

/**
 *@Desc: init gridlayout ,tool bar and command line
 *@Args: QWidget *(parent ), bool(view only mode), int (max input buffer size), int(max output buffer size)
 *@Returns: None
 */
MarsConsole::MarsConsole(QWidget *parent,bool viewOnly,int maxIBufferSize,int maxOBufferSize) : QWidget(parent)
{    


    layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);
    // note!!!: createToolBar() must be called before createCommandLine(*);
    createToolBar();
    createCommandLine(1,viewOnly,maxIBufferSize,maxOBufferSize);

}


/**
 *@Desc: create tool bar
 *@Args: None
 *@Returns: None
 *
 */
void MarsConsole::createToolBar()
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
    toolBar->setFixedHeight(25);
    toolBar->setContentsMargins(0,0,0,0);
    toolBar->setOrientation(Qt::Horizontal);
    exportDataAction = new QAction(QIcon(":/icon/export"),tr("导出数据"),this);
    importDataAction = new QAction(QIcon(":/icon/import"),tr("导入数据"),this);
    toolBar->addAction(exportDataAction);
    toolBar->addAction(importDataAction);
    connect(exportDataAction,&QAction::triggered,this,&MarsConsole::exportData);
    connect(importDataAction,&QAction::triggered,this,&MarsConsole::importData);
    layout->addWidget(toolBar,1,1);
}

/**
 *@Desc: create command line,when create successfully return true otherwise return false
 *@Args: int(command line number), bool(view only), int (command line max input buffer size)
 * , int(command line max output buffer size)
 *@Returns: bool
 */
bool MarsConsole::createCommandLine(int number,bool viewOnly,int maxIBufferSize, int maxOBufferSize)
{

    MarsCommandLine * tmpCmdLine;
    for(int i=0;i<number;++i)
    {
        if(commandLines.size()>=4)
            break;
        tmpCmdLine = new MarsCommandLine(this,viewOnly, maxIBufferSize, maxOBufferSize);
        connect(tmpCmdLine,&MarsCommandLine::dataExportRequest,this,&MarsConsole::exportData);
        connect(tmpCmdLine,&MarsCommandLine::dataImportRequest,this,&MarsConsole::importData);
        connect(tmpCmdLine,&MarsCommandLine::dataIn,this,&MarsConsole::onCommandLineDataReady);
        commandLines.append(tmpCmdLine);
    }
    switch(commandLines.size())
    {
    case 1:
        layout->addWidget(commandLines.at(0),2,1);
        break;
    case 2:
        layout->addWidget(commandLines.at(0),2,1);
        layout->addWidget(commandLines.at(1),2,2);
        commandLine(0).style()->addStyleSheet("border-right:1px solid #666;");
        commandLine(1).style()->addStyleSheet("border-left:1px solid #666;");
        break;
    case 3:
        layout->addWidget(commandLines.at(0),2,1);
        layout->addWidget(commandLines.at(1),2,2);
        layout->addWidget(commandLines.at(2),3,1,1,2);
        commandLine(0).style()->addStyleSheet("border-right:1px solid #666;border-bottom:1px solid #666;");
        commandLine(1).style()->addStyleSheet("border-left:1px solid #666;border-bottom:1px solid #666;");
        commandLine(2).style()->addStyleSheet("border-top:1px solid #666;");
        break;
    case 4:
        layout->addWidget(commandLines.at(0),2,1);
        layout->addWidget(commandLines.at(1),2,2);
        layout->addWidget(commandLines.at(2),3,1);
        layout->addWidget(commandLines.at(3),3,2);
        commandLine(0).style()->addStyleSheet("border-right:1px solid #666;border-bottom:1px solid #666;");
        commandLine(1).style()->addStyleSheet("border-left:1px solid #666;border-bottom:1px solid #666;");
        commandLine(2).style()->addStyleSheet("border-right:1px solid #666;border-top:1px solid #666;");
        commandLine(3).style()->addStyleSheet("border-left:1px solid #666;border-top:1px solid #666;");
        break;
    }
    if(commandLines.size()>0)
    {
        if(viewOnly)
            importDataAction->setEnabled(false);
    }
    return true;
}

/**
 *@Desc: delete created command line instance and layout
 *@Args: None
 *@Returns: None
 */
MarsConsole::~MarsConsole()
{
    for(int i=0;i<commandLines.size();++i)
    {
        delete (commandLines.at(i));
    }
    delete layout;
}

/**
 *@Desc: return specific command line instance reference
 *@Args: int( command line instance index)
 *@Return: MarsCommandLine &
 */
MarsCommandLine & MarsConsole::commandLine(int index)
{
    if(commandLines.size()==0)
    {
        createCommandLine(1);
        return *(commandLines.at(0));
    }
    if(commandLines.size()<(index-1))
        return *(commandLines.last());
    return *(commandLines.at(index));
}

/**
 *@Desc: slot function for command line input data ready signal
 *@Args: None
 *@Returns: None
 */
void MarsConsole::onCommandLineDataReady()
{
    emit dataReady();
}

/**
 *@Desc: slot function for saving command  data  to a file
 *@Args: None
 *@Returns: None
 */
void MarsConsole::exportData()
{
    QString fullFileName = QFileDialog::getSaveFileName(this,tr("保存为....."),QString(),
                                                    tr("text files(*.txt);;json files(*.json);; xml files(*.xml)"));
    if(fullFileName.isEmpty())
    {
        emit error(errorInstance(tr("文件名不能为空"),ERROR));
        return ;
    }
    QFile file(fullFileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QString shortFileName = (fullFileName.split(QRegExp("[/\\]+"))).last();
        emit error(errorInstance(shortFileName+"打开失败",ERROR));
        return ;
    }
    QString fileType =fullFileName.split('.').last();
    fileType = fileType.toLower();
    try{

        if(fileType=="json")
        {
            writeJSONFile(&file);
        }
        else if(fileType =="xml")
        {
            writeXMLFile(&file);
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
 *@Desc: return error instance
 *@Args:  QString ,MarsErrorLevel
 *@Returns: MarsError
 */
MarsError MarsConsole::errorInstance(QString msg, MarsErrorLevel level)
{
    MarsError error;
    error.datetime = QDateTime::currentDateTime();
    error.type = CONSOLE;
    error.msg = msg;
    error.level = level;
    return error;
}

/**
 *@Desc: slot function for loading file data into command line input buffer
 *@Args: None
 *@Returns: None
 */
void MarsConsole::importData()
{
    QString fullFileName = QFileDialog::getOpenFileName(this,tr("导入文件"),QString(),
                                     tr("text files(*.txt);;json files(*.json);; xml files(*.xml)"));
    if(fullFileName.isEmpty())
    {

        emit error(errorInstance(tr("文件名不能为空"),ERROR));
        return ;
    }
    QFile file(fullFileName);
    QString shortFileName = (fullFileName.split(QRegExp("[/\\]+"))).last();
    if(!file.open(QIODevice::ReadOnly))
    {
        emit error(errorInstance(shortFileName+"打开失败",ERROR));
        return ;
    }
    // max file size 50M
    if(file.size()>1024*1024*50)
    {
        emit error(errorInstance(shortFileName+"大小已超过50M",ERROR));
        return ;
    }
    QString fileType =shortFileName.split('.').last();
    fileType = fileType.toLower();
    try{

        if(fileType=="json")
        {
            readJSONFile(&file);
        }
        else if(fileType =="xml")
        {
            readXMLFile(&file);
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
 *@Desc: read text file and import data into command line input buffer
 *@Args: QFile *
 *@Returns: None
 */
void MarsConsole::readTextFile(QFile * file)
{
    QTextStream stream(file);
    commandLine(0)<<stream;
}

/**
 *@Desc: write output buffer's data into text file
 *@Args: QFile *
 *@Returns: None
 */
void MarsConsole::writeTextFile(QFile * file)
{
    QTextStream stream(file);
    stream<<commandLine(0).outputBufferText();
}

/**
 *@Desc: read josn file and import data into  input buffer
 *@Args: QFile *
 *@Returns: None
 */
void MarsConsole::readJSONFile(QFile *file)
{

}

/**
 *@Desc: write output buffer's data into json file
 *@Args: QFile * file
 *@Returns: None
 */
void MarsConsole::writeJSONFile(QFile * file)
{

}

/**
 *@Desc: read xml file and import data into input buffer
 *@Args: QFile * file
 *@Returns: None
 */
void MarsConsole::readXMLFile(QFile * file)
{

}

/**
 *@Desc: write output buffer's data into  file
 *@Args: QFile *file
 *@Returns: None
 */
void MarsConsole::writeXMLFile(QFile * file)
{

}
