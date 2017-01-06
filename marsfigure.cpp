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
#include "marsplot.h"


MarsFigure::MarsFigure(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);

    // Note:!!!
    // initToolBar() must be called before initPlot(),
    // fuck ,yeah I have written this shit code !!!!!!!!!!!!!!!
    initToolBar();
    initPlot();
    connect(this,&MarsFigure::getImportFileName,this,&MarsFigure::readFile);
    connect(this,&MarsFigure::getExportFileName,this,&MarsFigure::writeFile);
}

MarsFigure::~MarsFigure()
{
    delete exportDataAction;
    delete importDataAction;
    delete plotAction;
    delete saveGraphAction;
    delete clearGraphAction;
    delete deletePloterAction;
    delete createPloterAction;
    delete ploterContainer;
    delete plotLayout;
    delete layout;
}

/**
 *@Desc: init tool bar
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
    createPloterAction = new QAction(QIcon(":/icon/create-plot"),tr("新建图像"),this);
    saveGraphAction = new QAction(QIcon(":/icon/save"),tr("保存图像"),this);
    exportDataAction = new QAction(QIcon(":/icon/export"),tr("导出数据"),this);
    importDataAction = new QAction(QIcon(":/icon/import"),tr("导入数据"),this);
    clearGraphAction = new QAction(QIcon(":/icon/clear"),tr("清除图像"),this);
    deletePloterAction = new QAction(QIcon(":/icon/delete-plot"),tr("删除图像"),this);
    ploterNameListBox =new QComboBox(this);
    ploterNameListBox->setStyleSheet("QComboBox{"
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

    toolBar->addAction(plotAction);
    toolBar->addAction(createPloterAction);
    toolBar->addSeparator();
    toolBar->addWidget(ploterNameListBox);
    toolBar->addAction(saveGraphAction);
    toolBar->addAction(exportDataAction);
    toolBar->addAction(importDataAction);
    toolBar->addAction(clearGraphAction);
    toolBar->addAction(deletePloterAction);
    connect(saveGraphAction,&QAction::triggered,this,&MarsFigure::saveGraph);
    connect(exportDataAction,&QAction::triggered,this,&MarsFigure::showExportDataDialog);
    connect(importDataAction,&QAction::triggered,this,&MarsFigure::showImportDataDialog);
    connect(plotAction,&QAction::triggered,this,&MarsFigure::onPlotActionTriggered);
    connect(clearGraphAction,&QAction::triggered,this,&MarsFigure::clearGraph);
    connect(ploterNameListBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCurrentPloter(int)));
    connect(createPloterAction,&QAction::triggered,this,&MarsFigure::createPloter);
    connect(deletePloterAction,&QAction::triggered,this,&MarsFigure::deletePloter);
    layout->addWidget(toolBar);
}

/**
 *@Desc: init plot
 *@Args: None
 *@Returns: None
 *
 */

void MarsFigure::initPlot()
{
    plotLayout = new QGridLayout();
    layout->addLayout(plotLayout);
    ploterContainer = new QList<QCustomPlot*>();
    currentPloter = createPloter();
    plotState = false;
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
    if(ploterContainer->length()>= MAX_PLOTER_NUMBER)
        return NULL;
    tmpPloter = new MarsPlot(this);
    tmpPloter->xAxis2->setVisible(true);
    tmpPloter->xAxis2->setTickLabels(false);
    tmpPloter->yAxis2->setVisible(true);
    tmpPloter->yAxis2->setTickLabels(false);
    tmpPloter->xAxis->setLabel("x");
    tmpPloter->xAxis->setLabelPadding(0);
    tmpPloter->yAxis->setLabel("y");
    tmpPloter->yAxis->setLabelPadding(0);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(tmpPloter->xAxis, SIGNAL(rangeChanged(QCPRange)), tmpPloter->xAxis2, SLOT(setRange(QCPRange)));
    connect(tmpPloter->yAxis, SIGNAL(rangeChanged(QCPRange)), tmpPloter->yAxis2, SLOT(setRange(QCPRange)));
    tmpPloter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ploterContainer->append(tmpPloter);
    currentPloter = tmpPloter;
    arrangePloter();
    updateStatusBar();
    return tmpPloter;
}

/**
 *@Desc: delete selected ploter
 *@Args: None
 *@Returns: None
 */
void MarsFigure::deletePloter()
{

    if(ploterContainer->length()<=1)
        return ;
    ploterContainer->removeAt(ploterContainer->indexOf(currentPloter));
    delete currentPloter;
    currentPloter = ploterContainer->last();
    arrangePloter();
    updateStatusBar();


}
/**
 *@Desc: update status bar actions when create ploter or delete ploter
 *@Args: None
 *@Returns: None
 */
void MarsFigure::updateStatusBar()
{

    /* enable or disable deletePloterAction */
    if(ploterContainer->length()==MIN_PLOTER_NUMBER)
         deletePloterAction->setEnabled(false);
    else
         deletePloterAction->setEnabled(true);
    /* enable or disable createPloterAction */
    if(ploterContainer->length()>=MAX_PLOTER_NUMBER)
         createPloterAction->setEnabled(false);
    else
         createPloterAction->setEnabled(true);
    /* readd ploterNameListBox */
    disconnect(ploterNameListBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCurrentPloter(int)));
    ploterNameListBox->clear();
    for(int i =0;i<ploterContainer->length();i++)
        ploterNameListBox->addItem("plot "+QString::number(i+1),i);
    ploterNameListBox->setCurrentIndex(ploterContainer->indexOf(currentPloter));
    connect(ploterNameListBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCurrentPloter(int)));

}
/**
 *@Desc: arrange ploter in parent layout
 *@Args: None
 *@Returns: None
 */
void MarsFigure::arrangePloter()
{
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
    for(int i=0;i<currentPloter->graphCount();i++)
    {
        currentPloter->graph(i)->data()->clear();
    }
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

/**
 *@Desc: fuck you, another plotting method
 *@Args: QList<QByteArray> & data
 *@Returns: None
 */
void MarsFigure::plot(QList<QByteArray> & data)
{
    QString dataStr = data.join('\n');
    plot(dataStr);
}

/**
 *@Desc: fuck you , another plotting method
 *@Args: QByteArray & data
 *@Returns: None
 *
 */
void MarsFigure::plot(QByteArray & data)
{
    QString dataStr = data;
    plot(dataStr);
}

/**
 *@Desc:
 *  plot graph with string data
 *  string data formation:
 *      each data line end with '\n',each data line contains several ' ' characters as a seperator.
 *
 *      generally speaking, it almost like the following formation:
 *              "x1 y^11 y^12 y^13"
 *
 *@Args: QString & data
 *@Returns: None
 */

/** yeah, I have written this shit code,fuck you ,the code */

/** strongly recommend, the data size is better less than 100 thousands line */
void MarsFigure::plot(QString &data)
{

     QStringList dataLineItems;
     QStringList dataLines;
     double x,y;
    /* ploting stopped */
    if(!plotState)
        return ;
    /* check data empty */
    if (data.isEmpty())
        return ;
    dataLines = data.split('\n');
    if(dataLines.length()==0)
        return ;
    /*
     * if graph doesn't exist,then create it
     */
    int dataGraphNumber = dataLines.at(0).split(" ").length()-1;
    int existedGraphNumber = currentPloter->graphCount();
    if(dataGraphNumber>existedGraphNumber)
    {
        for(int graphId=existedGraphNumber; graphId<dataGraphNumber;++graphId)
        {
            currentPloter->addGraph();
            currentPloter->graph(graphId)->setPen(QPen(graphColor(graphId)));
            currentPloter->graph(graphId)->rescaleAxes(false);
        }
    }
    for(int lineNumber=0;lineNumber<dataLines.length();++lineNumber)
    {
        dataLineItems = dataLines.at(lineNumber).split(" ");
        x = dataLineItems.at(0).toDouble();
        for(int graphId =0; graphId<dataLineItems.length()-1;++graphId)
        {
             y = dataLineItems.at(graphId+1).toDouble();
             currentPloter->graph(graphId)->addData(x,y);
        }
    }
    currentPloter->replot();

}
/**
 *@Desc:
 *  plot graph
 *  Note !!!:
 *      The graphId starts from 0 ,the same as plotId.
 *      If you pass a graphId or plotId variable which's value is greater than 3, the result
 *      is not affect.
 *
 *@Args:
 *  double x, double y , int graphId(max graphId is 3), int plotId(max plotId is 3)
 *@Returns:
 *  None
 */
void MarsFigure::plot(double x, double y, int graphId,int plotId)
{
    /* return back if plotState is false ,it indicate ploting stopped */
    if (!plotState)
        return ;
    if((ploterContainer->length()-1)<plotId)
    {
        if(!createPloter())
            return ;
        // recorrect plotId after create new ploter
        plotId = ploterContainer->length()-1;
    }
    if((graphId+1)>ploterContainer->at(plotId)->graphCount())
    {
        ploterContainer->at(plotId)->addGraph();
        // recorrent graphId after create new graph in the current ploter
        graphId = ploterContainer->at(plotId)->graphCount()-1;
        ploterContainer->at(plotId)->graph(graphId)->setPen(QPen(graphColor(graphId)));
        ploterContainer->at(plotId)->graph(graphId)->rescaleAxes(false);
    }
    ploterContainer->at(plotId)->graph(graphId)->addData(x,y);
    ploterContainer->at(plotId)->xAxis->setRange(x+10,100,Qt::AlignRight);
    ploterContainer->at(plotId)->replot();
}

/**
 *@Desc:
 *  plot data graph
 *  Note !!!:
 *      The graphId start from 0 ,the same as plotId.
 *      If you pass a graphId or plotId variable which's value is greater than 3,
 *      the result is not affected.
 *@Args:
 *  QVector<double> &x,QVector<double> &y,int graphId(max graphId is 3),
 *  int plotId( max plotId is 3)
 *@Return: None
 */
void MarsFigure::plot(QVector<double> &x, QVector<double> &y, int graphId,int plotId)
{


    /* return back if plotState is false ,it indicate ploting stopped */
    if (!plotState)
        return ;

    if((ploterContainer->length()-1)<plotId)
    {
        if(!createPloter())
            return ;
        // recorrect plotId after create new ploter
        plotId = ploterContainer->length()-1;
    }
    if((graphId+1)>ploterContainer->at(plotId)->graphCount())
    {
        ploterContainer->at(plotId)->addGraph();
        // recorrent graphId after create new graph in the current ploter
        graphId = ploterContainer->at(plotId)->graphCount()-1;
        ploterContainer->at(plotId)->graph(graphId)->setPen(QPen(graphColor(graphId)));
        ploterContainer->at(plotId)->graph(graphId)->rescaleAxes(false);
    }
    ploterContainer->at(plotId)->graph(graphId)->setData(x,y); // 65-67 ms /per hundred thousand data
  //  ploterContainer->at(plotId)->xAxis->setRange(x.last()-10,100,Qt::AlignRight);
    ploterContainer->at(plotId)->replot();
}

/**
 *@Desc: another overrid plotting method
 *@Args: QVector<double> & x,QVector<double> &y, int graphId
 *@Returns: None
 */
void MarsFigure::plot(QVector<double> &x, QVector<double> &y, int graphId)
{

    /* return back if plotState is false ,it indicate ploting stopped */
    if (!plotState)
        return ;
    if((graphId+1)>currentPloter->graphCount())
    {
        currentPloter->addGraph();
        // recorrent graphId after create new graph in the current ploter
        graphId = currentPloter->graphCount()-1;
        currentPloter->graph(graphId)->setPen(QPen(graphColor(graphId)));
        currentPloter->graph(graphId)->rescaleAxes(false);
    }
    currentPloter->graph(graphId)->setData(x,y); // 65-67 ms /per hundred thousand data
   // currentPloter->xAxis->setRange(x.last()-10,100,Qt::AlignRight);
    currentPloter->replot();
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
 *@Desc: show exporting data  dialog;
 *@Args: None
 *@Returns: None
 */
void MarsFigure::showExportDataDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("导出数据"),QString(),
                                                    tr("text files(*.txt);;json files(*.json);;binary files(*.dat);;xml files(*.xml)"));
    if(fileName.isEmpty())
    {
        emit error(errorInstance(tr("文件名不能为空"),ERROR));
        return ;
    }
    QTimer::singleShot(10,[=](){
        emit getExportFileName(fileName);
    });

}
/**
 *@Desc: write graph's data into file
 *@Args: const QString & fileName
 *@Returns: None
 */
void MarsFigure::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QString shortFileName = (fileName.split(QRegExp("[/\]+"))).last();
        emit error(errorInstance(shortFileName+"打开失败",ERROR));
        return ;
    }
    QString fileType =fileName.split('.').last();
    fileType = fileType.toLower();
    try{

        if(fileType == "json")
        {
            writeJSONFile(&file);
        }
        else if(fileType == "dat")
        {
            writeDatFile(&file);
        }
        else if(fileType == "xml")
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
 *@Desc: show importing data dialog
 *@Args: None
 *@Returns: None
 */
void MarsFigure::showImportDataDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("导入文件"),QString(),
                                     tr("text files(*.txt);;json files(*.json);;binary files(*.dat);;xml files(*.xml)"));
    if(fileName.isEmpty())
    {
        emit error(errorInstance(tr("文件名不能为空"),ERROR));
        return ;
    }
    QTimer::singleShot(5,[=](){
          emit getImportFileName(fileName);
    });
}

/**
 *@Desc: read file data to ploting
 *@Args: const QString & fileName
 *@Returns: None
 */
void MarsFigure::readFile(const QString &fileName)
{
    QFile file(fileName);
    QString shortFileName = (fileName.split(QRegExp("[/\]+"))).last();
    if(!file.open(QIODevice::ReadOnly))
    {
        emit error(errorInstance(shortFileName+"打开失败",ERROR));
        return ;
    }
    // max file size 20M
    if(file.size()>1024*1024*20)
    {
        emit error(errorInstance(shortFileName+"大小已超过20M",ERROR));
        return ;
    }
    QString fileType =shortFileName.split('.').last();
    fileType = fileType.toLower();
    try{
        if(fileType == "json")
        {

            readJSONFile(&file);
        }
        else if(fileType == "dat")
        {
            readDatFile(&file);
        }
        else if(fileType == "xml")
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
 *@Desc: reading text file and plot data's graph
 *@Args: QFile *
 *@Returns: None
 */
void MarsFigure::readTextFile(QFile * file)
{

    QTextStream stream(file);
    QString data = stream.readAll();
    clearGraph();
    startPlot();
    plot(data);

}

/**
 *@Desc: writting graph's data into text file
 *@Args: QFile *
 *@Returns: None
 */
void MarsFigure::writeTextFile(QFile * file)
{
    QTextStream out(file);
    int graphNumber = currentPloter->graphCount();
    if (graphNumber<=0)
        return ;
    int dataCount = currentPloter->graph(0)->dataCount();
    for( int j=0;j<dataCount;j++)
    {
        out<<currentPloter->graph(0)->dataMainKey(j);
        for(int i =0;i<graphNumber;i++)
        {
            out<<" "<<currentPloter->graph(i)->dataMainValue(j);
        }
        out<<"\n";
    }
}

/**
 *@Desc: reading josn file and plot data's graph
 *@Args: QFile *
 *@Returns: None
 */
void MarsFigure::readJSONFile(QFile *file)
{


    QByteArray saveData=file->readAll();
    /* load data from json file */
    QJsonDocument document=QJsonDocument::fromJson(saveData);
    QJsonArray xArray=document.object().take("x").toArray();
    QJsonArray yArray=document.object().take("y").toArray();
    int size = xArray.size();
    QVector<double> xVector(size);
    QList<QVector<double> *> yVector;
    QList<QJsonArray> yArrayList;
    int graphNumber = yArray.size();
    /* check y axis value */
    for(int i=0;i<graphNumber;++i)
    {
        yArrayList.append(yArray.at(i).toArray());
        yVector.append(new QVector<double>(size));
    }
    /* convert json array to vector */
    for(int i=0;i<size;i++)
    {
        xVector.append(xArray.at(i).toDouble());
        for(int j =0;j<graphNumber;j++)
        {
            yVector.at(j)->append(yArrayList.at(j).at(i).toDouble());
        }
    }
    /* start ploting data */
    clearGraph();
    startPlot();

    for(int i=0;i<graphNumber;i++)
    {
        QVector<double> * y = yVector.at(i);
        plot(xVector,*y,i);
        delete y;
    }

}

/**
 *@Desc: writting graph's data into json file
 *@Args: QFile * file
 *@Returns: None
 */
void MarsFigure::writeJSONFile(QFile * file)
{
    int graphNumber = currentPloter->graphCount();
    if (graphNumber<=0)
        return ;
    QJsonObject data;
    QJsonArray yValue;
    QVariantList xValueList;
    QList<QVariantList*> yValueList;
    for(int i =0;i<graphNumber;i++)
    {
        yValueList.append(new QVariantList);
    }
    int dataCount = currentPloter->graph(0)->dataCount();
    for( int j=0;j<dataCount;j++)
    {
        xValueList.append(currentPloter->graph(0)->dataMainKey(j));
        for(int i =0;i<graphNumber;i++)
        {
           yValueList.at(i)->append(currentPloter->graph(i)->dataMainValue(j));
        }
    }
    /* convert y axis value to VariantList */
    for(int i =0;i<graphNumber;i++)
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
}
/**
 *@Desc: readding dat file and draw data's graph
 *@Args: QFile * file
 *@Returns: None
 */
void MarsFigure::readDatFile(QFile * file)
{
    QByteArray saveData=file->readAll();
    /* load data from dat file */
    QJsonDocument document=QJsonDocument::fromBinaryData(saveData);
    QJsonArray xArray=document.object().take("x").toArray();
    QJsonArray yArray=document.object().take("y").toArray();
    int size = xArray.size();
    QVector<double> xVector(size);
    QList<QVector<double> *> yVector;
    QList<QJsonArray> yArrayList;
    int graphNumber = yArray.size();
    /* check y axis value */
    for(int i=0;i<graphNumber;++i)
    {
        yArrayList.append(yArray.at(i).toArray());
        yVector.append(new QVector<double>(size));
    }
    /* convert json array to vector */
    for(int i=0;i<size;i++)
    {
        xVector.append(xArray.at(i).toDouble());
        for(int j =0;j<graphNumber;j++)
        {
            yVector.at(j)->append(yArrayList.at(j).at(i).toDouble());
        }
    }
    /* start ploting data */
    clearGraph();
    startPlot();
    for(int i=0;i<graphNumber;i++)
    {
        QVector<double> * y = yVector.at(i);
        plot(xVector,*y,i);
        delete y;
    }
}
/**
 *@Desc: writting graph's data into dat file
 *@Args: QFile * file
 *@Returns: None
 */
void MarsFigure::writeDatFile(QFile * file)
{
    int graphNumber = currentPloter->graphCount();
    if (graphNumber<=0)
        return ;
    QJsonObject data;
    QJsonArray yValue;
    QVariantList xValueList;
    QList<QVariantList*> yValueList;
    for(int i =0;i<graphNumber;i++)
    {
        yValueList.append(new QVariantList);
    }
    int dataCount = currentPloter->graph(0)->dataCount();
    for( int j=0;j<dataCount;j++)
    {
        xValueList.append(currentPloter->graph(0)->dataMainKey(j));
        for(int i =0;i<graphNumber;i++)
        {
           yValueList.at(i)->append(currentPloter->graph(i)->dataMainValue(j));
        }
    }
    /* convert y axis value to VariantList */
    for(int i =0;i<graphNumber;i++)
    {
        yValue.append(QJsonArray::fromVariantList(*(yValueList.at(i))));
        delete yValueList.at(i);
    }
    /* convert x axis value variant list to json array*/
    data["x"]=QJsonArray::fromVariantList(xValueList);
    data["y"]=yValue;
    /* writing json document to binary file */
    QJsonDocument saveDocument(data);
    file->write(saveDocument.toBinaryData());
}
/**
 *@Desc: reading xml file and draw data's graph
 *@Args: QFile * file
 *@Returns: None
 */
void MarsFigure::readXMLFile(QFile * file)
{
    /*
     *
     * reading data from xml file.
     * I not decide to implement it at this time,so just let it fuck here ,byebye
     */
}
/**
 *@Desc: writting graph's data to xml file
 *@Args: QFile *file
 *@Returns: None
 */
void MarsFigure::writeXMLFile(QFile * file)
{
    /*
     * writing data into xml file.
     * I also not decide to implement it at this time, it fuck me all the timea, son of bitch
     */
}

