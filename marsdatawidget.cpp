#include "marsdatawidget.h"
#include <QFile>
#include <QAction>
#include <QToolBar>
#include <QGridLayout>
#include <QFileDialog>


MarsDataWidget::MarsDataWidget(QWidget *parent):QWidget(parent)
{
    layout  = new QGridLayout(this);
    createToolBar();
}

MarsDataWidget::~MarsDataWidget()
{
    delete toolBarIns;
    delete layout;
}

void MarsDataWidget::createToolBar()
{
    toolBarIns = new QToolBar("hello",this);
    toolBarIns->setStyleSheet(
                           "margin:0px 0px 0px 0px;"
                           "padding:0px 0px 0px 0px;"
                           "background-color:#666;"
                           "border-left:1px solid #333;"
                           "spacing:5px;"

                           );

    toolBarIns->setAutoFillBackground(true);
    toolBarIns->setFixedHeight(25);
    toolBarIns->setContentsMargins(0,0,0,0);
    toolBarIns->setOrientation(Qt::Horizontal);
    layout->addWidget(toolBarIns,1,1);
}

/**
 *@Desc: slot function for saving command  data  to a file
 *@Args: None
 *@Returns: None
 */
void MarsDataWidget::exportData()
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
 *@Desc: slot function for loading file data into command line input buffer
 *@Args: None
 *@Returns: None
 */
void MarsDataWidget::importData()
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
 *@Desc: return error instance
 *@Args:  QString ,MarsErrorLevel
 *@Returns: MarsError
 */
MarsError MarsDataWidget::errorInstance(QString msg, MarsErrorLevel level)
{
    MarsError error;
    error.datetime = QDateTime::currentDateTime();
    error.type = CONSOLE;
    error.msg = msg;
    error.level = level;
    return error;
}

QToolBar & MarsDataWidget::toolBar()
{
    return *toolBarIns;
}


void MarsDataWidget::readTextFile(QFile * file)
{
    /* subclass implement it */
}

void MarsDataWidget::readJSONFile(QFile *file)
{
    /* subclass implement it */
}

void MarsDataWidget::readXMLFile(QFile *file)
{
    /* subclass implement it */
}

void MarsDataWidget::writeTextFile(QFile *file)
{
    /* subclass implement it */
}

void MarsDataWidget::writeJSONFile(QFile *file)
{
    /* subclass implement it */
}

void MarsDataWidget::writeXMLFile(QFile *file)
{
    /* subclass implement it */
}
