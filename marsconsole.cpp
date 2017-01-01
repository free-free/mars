#include "marsconsole.h"
#include "marscommandline.h"
#include <QTextStream>
#include <QFileDialog>
#include <QGridLayout>
#include <QDebug>
#include <QList>
#include <QToolBar>

MarsConsole::MarsConsole(QWidget *parent,bool view_only,int maxIBufferSize,int maxOBufferSize) : QWidget(parent)
{    


    layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);
    createToolBar();
    createCommandLine(1,view_only,maxIBufferSize,maxOBufferSize);


}


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
    saveAsAction = new QAction(QIcon(":/icon/export"),tr("导出数据"),this);
    loadInAction = new QAction(QIcon(":/icon/import"),tr("导入数据"),this);
    toolBar->addAction(saveAsAction);
    toolBar->addAction(loadInAction);
    layout->addWidget(toolBar,1,1);
}
bool MarsConsole::createCommandLine(int number,bool view_only,int maxIBufferSize, int maxOBufferSize)
{

    MarsCommandLine * tmpCmdLine;
    for(int i=0;i<number;++i)
    {
        if(commandLines.size()>=4)
            break;
        tmpCmdLine = new MarsCommandLine(this,view_only, maxIBufferSize, maxOBufferSize);
        connect(tmpCmdLine,&MarsCommandLine::dataSaveRequest,this,&MarsConsole::saveDataAs);
        connect(tmpCmdLine,&MarsCommandLine::dataLoadRequest,this,&MarsConsole::loadDataIn);
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
    return true;
}
MarsConsole::~MarsConsole()
{
    for(int i=0;i<commandLines.size();++i)
    {
        delete (commandLines.at(i));
    }
    delete layout;
}

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
void MarsConsole::onCommandLineDataReady()
{
    emit dataReady();
}

void MarsConsole::saveDataAs()
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

void MarsConsole::loadDataIn()
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


void MarsConsole::readTextFile(QFile * file)
{
    QTextStream stream(file);
    commandLine(0)<<stream;
}

void MarsConsole::writeTextFile(QFile * file)
{
    QTextStream stream(file);
    stream<<commandLine(0).outputBufferText();
}

void MarsConsole::readJSONFile(QFile *file)
{

}

void MarsConsole::writeJSONFile(QFile * file)
{

}

void MarsConsole::readXMLFile(QFile * file)
{

}

void MarsConsole::writeXMLFile(QFile * file)
{

}
