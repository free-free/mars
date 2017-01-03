#include "marsfigure.h"
#include <QAction>
#include <QGridLayout>
#include <QFile>
#include <QFileDialog>
#include <QToolBar>
#include <QList>
#include <QComboBox>
#include <QVBoxLayout>
#include "qcustomplot.h"

MarsFigure::MarsFigure(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);
    // Note:!!!
    // initToolBar() must be called before initPlot(),
    // fuck ,yeah i have written this shit code !!!!!!!!!!!!!!!
    initToolBar();
    initPlot();

}

MarsFigure::~MarsFigure()
{
    delete exportDataAction;
    delete importDataAction;
    delete plotAction;
    delete saveGraphAction;
    delete clearGraphAction;
    delete ploterContainer;
    delete layout;
}

/**
 *@Desc: create tool bar
 *@Args: None
 *@Returns: None
 *
 */
void MarsFigure::initToolBar()
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
    plotAction = new QAction(QIcon(":/icon/start"),tr("开始绘制"),this);
    saveGraphAction = new QAction(QIcon(":/icon/save"),tr("保存图像"),this);
    exportDataAction = new QAction(QIcon(":/icon/export"),tr("导出数据"),this);
    importDataAction = new QAction(QIcon(":/icon/import"),tr("导入数据"),this);
    clearGraphAction = new QAction(QIcon(":/icon/clear"),tr("清除图像"),this);
    plotNameListBox =new QComboBox(this);
    plotNameListBox->setStyleSheet("QComboBox{"
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
                         "QComboBox::on{"
                            "background-color:#f1f2f3;"
                            "color:#666;"
                         "}"
                         );

    toolBar->addAction(plotAction);
    toolBar->addSeparator();
    toolBar->addWidget(plotNameListBox);
    toolBar->addAction(saveGraphAction);
    toolBar->addAction(exportDataAction);
    toolBar->addAction(importDataAction);
    toolBar->addAction(clearGraphAction);
    connect(saveGraphAction,&QAction::triggered,this,&MarsFigure::saveGraph);
    connect(exportDataAction,&QAction::triggered,this,&MarsFigure::exportData);
    connect(importDataAction,&QAction::triggered,this,&MarsFigure::importData);
    connect(plotAction,&QAction::triggered,this,&MarsFigure::onPlotActionTriggered);
    connect(clearGraphAction,&QAction::triggered,this,&MarsFigure::clearGraph);
    connect(plotNameListBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCurrentPloter(int)));
    layout->addWidget(toolBar);
}


/**
 *@Desc: start plot(including set plot state to true ,call replot function  and emit plotStarted signal
 *@Args: None
 *@Returns: None
 */
void MarsFigure::startPlot()
{
    plotState=true;
    int ploterNumber = ploterContainer->length();
    for(int i=0;i<ploterNumber;++i)
    {
       ploterContainer->at(i)->replot();
    }

    emit plotStarted();
}

/**
 *@Desc: stop plot(including set plot state to false  and emit plotStopped signal
 *@Args: None
 *@Returns: None
 */

void MarsFigure::stopPlot()
{
    plotState = false;
    emit plotStopped();
}

/**
 *@Desc: change plotAction's icon and text ,call stopPlot function
 *@Args: None
 *@Returns: None
 *
 */
void MarsFigure::onPlotActionTriggered()
{
    if(plotState)
    {
        plotAction->setIcon(QIcon(":/icon/start"));
        plotAction->setText(tr("开始绘制"));
        stopPlot();
    }
    else
    {
        plotAction->setIcon(QIcon(":/icon/pause"));
        plotAction->setText(tr("停止绘制"));
        startPlot();
    }
}

/**
 *@Desc: change current Ploter
 *@Args: int (ploter index in ploterContainer)
 *@Returns: None
 */
void MarsFigure::changeCurrentPloter(int index)
{
    currentPloter = ploterContainer->at(index);
}

/**
 *@Desc: clear plot
 *@Args: None
 *@Returns: None
 */
void MarsFigure::clearGraph()
{
    currentPloter->clearGraphs();
    currentPloter->replot();
}

/**
 *@Desc: generate graph color and return color
 *@Args: int (graph id)
 *@Returns: QColor
 */
QColor MarsFigure::graphColor(int graphId)
{
    switch(graphId)
    {
        case 0:
            return Qt::blue;
        case 1:
            return Qt::green;
        case 2:
            return Qt::red;
        case 3:
            return Qt::yellow;
        default:
            return Qt::blue;
    }
}


void MarsFigure::plot(QList<QByteArray> & data)
{

}

void MarsFigure::plot(QByteArray & data)
{

}

void MarsFigure::plot(QString &data)
{
    /*
    QStringList dataLines = data.split('\n');
    QStringList dataLineItems;
    QStringList dataVector;
    int graphId = 0;
    if(dataLines.length()>0)
    {
        int maxGraphId = dataLines.at(0).split(';').length()-1;
        if((maxGraphId+1)>ploter->graphCount())
        {
            for(int i=0;i<(maxGraphId-ploter->graphCount()+1);++i)
            {
                graphId = i+ploter->graphCount();
                ploter->addGraph();
                ploter->graph()->setPen(QPen(graphColor(graphId)));
                ploter->graph(graphId)->rescaleAxes(false);
            }

        }

    }
    for(int j=0;j<dataLines.length();++j)
    {
        dataLineItems = dataLines.at(j).split(';');
        for(int i=0;i<dataLineItems.length();++i)
        {
            dataVector = dataLineItems.at(i).split(',');
            ploter->graph(i)->addData(dataVector[0].toDouble(),dataVector[1].toDouble());
        }
    }
    if(plotState)
        ploter->replot();
    */
}

void MarsFigure::plot(int graphId,double x, double y)
{

    if((graphId+1)>currentPloter->graphCount())
    {
        currentPloter->addGraph();
        currentPloter->graph(graphId)->setPen(QPen(graphColor(graphId)));
        //ploter->graph(graphId)->rescaleAxes(false);
    }

    currentPloter->graph(graphId)->addData(x,y);
    //ploter->graph(graphId)->rescaleValueAxis();
    //ploter->graph(graphId)->rescaleKeyAxis();
    currentPloter->xAxis->setRange(x+10,100,Qt::AlignRight);
    if(plotState)
        currentPloter->replot();

}

void MarsFigure::plot(int graphId, QVector<double> &x,QVector<double> &y)
{
    /*
    graphId = abs(graphId);
    // if no such graph id,then create id
    if((graphId+1)>ploter->graphCount())
    {
        ploter->addGraph();
        ploter->graph(graphId)->setPen(QPen(graphColor(graphId)));
        ploter->graph(graphId)->rescaleAxes(false);
    }
    ploter->graph(graphId)->addData(x,y);
    if(plotState)
        ploter->replot();
     */
}



/**
 *@Desc: create ploter instance and config it ,finally append it to ploterContainter
 *@Args: None
 *@Returns: QCustomPlot *
 *
 */
QCustomPlot* MarsFigure::createPloter()
{

    QCustomPlot * tmpPloter;
    // greater than max ploter number
    if(ploterContainer->length()> MAX_PLOTER_NUMBER)
        return NULL;
    tmpPloter = new QCustomPlot(this);
    tmpPloter->xAxis2->setVisible(true);
    tmpPloter->xAxis2->setTickLabels(false);
    tmpPloter->yAxis2->setVisible(true);
    tmpPloter->yAxis2->setTickLabels(false);
    tmpPloter->xAxis->setLabel("x");
    tmpPloter->yAxis->setLabel("y");
    tmpPloter->xAxis->setLabelPadding(0);
    tmpPloter->yAxis->setLabelPadding(0);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(tmpPloter->xAxis, SIGNAL(rangeChanged(QCPRange)), tmpPloter->xAxis2, SLOT(setRange(QCPRange)));
    connect(tmpPloter->yAxis, SIGNAL(rangeChanged(QCPRange)), tmpPloter->yAxis2, SLOT(setRange(QCPRange)));

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    //ploter->graph(0)->rescaleAxes(false);
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    //ploter->graph(1)->rescaleAxes(true);
    // Note: we could have also just called plot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    tmpPloter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ploterContainer->append(tmpPloter);
    switch(ploterContainer->length())
    {
        case 1:
            plotLayout->addWidget(ploterContainer->at(0),1,1);
            break;
        case 2:
            plotLayout->addWidget(ploterContainer->at(0),1,1);
            plotLayout->addWidget(ploterContainer->at(1),2,1);
            break;
        case 3:
            plotLayout->addWidget(ploterContainer->at(0),1,1,1,2);
            plotLayout->addWidget(ploterContainer->at(1),2,1);
            plotLayout->addWidget(ploterContainer->at(2),2,2);
            break;
        case 4:
            plotLayout->addWidget(ploterContainer->at(0),1,1);
            plotLayout->addWidget(ploterContainer->at(1),2,1);
            plotLayout->addWidget(ploterContainer->at(2),2,2);
            plotLayout->addWidget(ploterContainer->at(3),1,2);
            break;
        default:
            break;
    }
    /* get new created ploter's id */
    int ploterId = ploterContainer->length()-1;
    /* add new ploter name to plotNameListBox with ploter id as data */
    plotNameListBox->addItem("plot "+(ploterId),ploterId);
    return tmpPloter;
}

/**
 *@Desc: return error instance
 *@Args:  QString ,MarsErrorLevel
 *@Returns: MarsError
 */
MarsError MarsFigure::errorInstance(QString msg, MarsErrorLevel level)
{
    MarsError error;
    error.datetime = QDateTime::currentDateTime();
    error.type = CONSOLE;
    error.msg = msg;
    error.level = level;
    return error;
}


/**
 *@Desc: slot function for saving   data  to a file
 *@Args: None
 *@Returns: None
 */
void MarsFigure::exportData()
{
    QString fullFileName = QFileDialog::getSaveFileName(this,tr("导出数据"),QString(),
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
        emit error(errorInstance("导出数据失败",ERROR));
    }


}

/**
 *@Desc: slot function for loading file data into command line input buffer
 *@Args: None
 *@Returns: None
 */
void MarsFigure::importData()
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
        emit error(errorInstance("导入"+shortFileName+"文件数据失败",ERROR));
    }
}

/**
 *@Desc: save plot graph as a file
 *@Args: None
 *@Return: None
 *
 */
void MarsFigure::saveGraph()
{
    QString fullFileName = QFileDialog::getSaveFileName(this,tr("保存图像"),QString(),
                tr("pdf files(*.pdf);;jpg files(*.jpg);; png files(*.png);; bmp file(*.bmp);;"));
    if(fullFileName.isEmpty())
    {
        emit error(errorInstance(tr("文件名不能为空"),ERROR));
        return ;
    }
    QString fileType =fullFileName.split('.').last();
    fileType = fileType.toLower();
    if(fileType=="pdf")
    {
        currentPloter->savePdf(fullFileName);
    }
    else if(fileType =="jpg")
    {
        currentPloter->saveJpg(fullFileName);
    }
    else if(fileType == "png")
    {
        currentPloter->savePng(fullFileName);
    }
    else if(fileType=="bmp")
    {
        currentPloter->saveBmp(fullFileName);
    }
    else
    {
        currentPloter->saveJpg(fullFileName);
    }

}
/**
 *@Desc: read text file and import data into input buffer
 *@Args: QFile *
 *@Returns: None
 */
void MarsFigure::readTextFile(QFile * file)
{

}

/**
 *@Desc: write output buffer's data into text file
 *@Args: QFile *
 *@Returns: None
 */
void MarsFigure::writeTextFile(QFile * file)
{

}

/**
 *@Desc: read josn file and import data into  input buffer
 *@Args: QFile *
 *@Returns: None
 */
void MarsFigure::readJSONFile(QFile *file)
{

}

/**
 *@Desc: write output buffer's data into json file
 *@Args: QFile * file
 *@Returns: None
 */
void MarsFigure::writeJSONFile(QFile * file)
{

}

/**
 *@Desc: read xml file and import data into input buffer
 *@Args: QFile * file
 *@Returns: None
 */
void MarsFigure::readXMLFile(QFile * file)
{

}

/**
 *@Desc: write output buffer's data into  file
 *@Args: QFile *file
 *@Returns: None
 */
void MarsFigure::writeXMLFile(QFile * file)
{

}
