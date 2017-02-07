/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: marsplot.cpp
 *@description: this file contains implementation of plot class
 *@version: 0.1
 *@author: jell
 *@date: 2017/01/12
 *
 *
 */

#include "marsplot.h"
#include "qcustomplot.h"
#include <QWheelEvent>
#include <QDebug>


/**
 *@Desc: ***
 *@Args: None
 *@Returns: None
 */
MarsPlot::MarsPlot(QWidget * parent):QCustomPlot(parent)
{
    titleText = "Graph Title";
    QCPTextElement *title = new QCPTextElement(this, titleText,
          QFont("sans",14));
    this->plotLayout()->insertRow(0);
    this->plotLayout()->addElement(0, 0, title);
    connect(this, &MarsPlot::legendDoubleClick,
            this, &MarsPlot::changeGraphLegendName);
    connect(this, &MarsPlot::axisDoubleClick,
            this, &MarsPlot::changeAxisLabelName);
    connect(title, &QCPTextElement::doubleClicked,
            this, &MarsPlot::changeTitle);
    connect(this, &MarsPlot::mouseMove,
            this, &MarsPlot::onMouseMove);
    xMaxRangeValue = 300;
    yMaxRangeValue = 20;
}


/**
 *@Desc: do thing here
 *@Args: None
 *@Return: None
 *
 */
MarsPlot::~MarsPlot()
{
    /* hi,buddy you can fuck here */
}


/**
 *@Desc: override QCustomPlot's wheelEvent to handle xAxis zooming and yAxis zooming seperately
 *@Args: QWheelEvent *
 *@Returns: None
 *
 */
void MarsPlot::wheelEvent(QWheelEvent *event)
{

    if(QApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
         axisRect()->setRangeZoom(Qt::Horizontal);
         if(event->delta()>0)
         {
            xMaxRangeValue -= 5.0;
            if(xMaxRangeValue < 0)
                xMaxRangeValue = 0;
         }
         else
         {
            xMaxRangeValue += 5.0;
         }
    }
    else if(QApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier))
    {

         axisRect()->setRangeZoom(Qt::Vertical);
         if(event->delta()>0)
         {
            yMaxRangeValue -= 0.3;
            if(yMaxRangeValue < 0)
                yMaxRangeValue = 0;
         }
         else
         {
            yMaxRangeValue += 0.3;
         }
    }
    else
    {
        axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);

    }
    QCustomPlot::wheelEvent(event);

}


void MarsPlot::focusInEvent(QFocusEvent *event)
{
    if(event->gotFocus())
    {
        emit focusIn(this);
    }
}


/**
 *@Desc: change title text of plotting
 *@Args: QMouseEvent * event
 *@Returns: None
 */
void MarsPlot::changeTitle(QMouseEvent * event)
{

    Q_UNUSED(event);
    if(QCPTextElement * title = qobject_cast<QCPTextElement*>(sender()))
    {
        bool ok;
        QString newTitle = QInputDialog::getText(this, tr("change title"),
            tr("New Title:"),QLineEdit::Normal,title->text(),&ok);
        if(ok)
        {
            titleText = newTitle;
            title->setText(titleText);
            this->replot();
        }
    }
}


/**
 *@Desc: change legend name of graph when doudle clicked on legend item
 *@Args: QCPLegend * legend,QCPAbstractLegendItem * item
 *@Return: None
 */
void MarsPlot::changeGraphLegendName(QCPLegend *legend, QCPAbstractLegendItem *item)
{

     Q_UNUSED(legend);
     if (item)
     {
       bool ok;
       QCPPlottableLegendItem *clickedItem = qobject_cast<QCPPlottableLegendItem*>(item);
       QString newName = QInputDialog::getText(this, tr("change graph name"), tr("New Graph Name:"),
                  QLineEdit::Normal, clickedItem->plottable()->name(), &ok);
       if(ok)
       {
           clickedItem->plottable()->setName(newName);
           this->replot();
       }
     }
}


/**
 *@Desc: change axis label name when double click on axis
 *@Args: QCPAxis * axis,QCPAxis::SelectablePart part
 *@Return: None
 */
void MarsPlot::changeAxisLabelName(QCPAxis * axis,QCPAxis::SelectablePart part)
{
    if(part == QCPAxis::spAxisLabel)
    {
        bool ok;
        QString newLabelName = QInputDialog::getText(this,tr("change axis label name"),
                tr("New Axis Label Name:"),QLineEdit::Normal,axis->label(),&ok);
        if(ok)
        {
            axis->setLabel(newLabelName);
            this->replot();
        }
    }
}


/**
 *@Desc: display x value and y value in title area when mouse is moving
 *@Args: QMouseEvent * event
 *@Return: None
 */
void MarsPlot::onMouseMove(QMouseEvent * event)
{
    double key, value ;
    QPointF pos(event->x(),event->y());
    QString data;
    if(plottableAt(pos))
    {

        plottableAt(pos)->pixelsToCoords(pos, key, value);
        data="x: "+QString::number(key)+" y: "+QString::number(value);
    }
    else
    {
        data = titleText;
    }
    QCPTextElement * title = (QCPTextElement*)this->plotLayout()->element(0,0);
    title->setText(data);
    this->replot();

}


/*
 *@Desc: return x axis max range value
 *@Args: None
 *@Returns: None
 */
double MarsPlot::xMaxRange() const
{
    return xMaxRangeValue ;
}


/*
 *@Desc: return y axis max range value
 *@Args: None
 *@Returns: None
 */
double MarsPlot::yMaxRange() const
{
    return yMaxRangeValue;
}
