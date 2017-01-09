#include "marsplot.h"
#include "qcustomplot.h"
#include <QWheelEvent>
#include <QDebug>

/**
 *@Desc: do no thing here
 *@Args: None
 *@Returns: None
 */
MarsPlot::MarsPlot(QWidget * parent):QCustomPlot(parent)
{
    /* hi,buddy you can fuck here */
    QCPTextElement *title = new QCPTextElement(this, "Graph Title",
          QFont("sans",14));
    this->plotLayout()->insertRow(0);
    this->plotLayout()->addElement(0, 0, title);
    connect(this, &MarsPlot::legendDoubleClick,
            this, &MarsPlot::changeGraphLegendName);
    connect(this, &MarsPlot::axisDoubleClick,
            this, &MarsPlot::changeAxisLabelName);
    connect(title, &QCPTextElement::doubleClicked,
            this, &MarsPlot::changeTitle);
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
    }
    else if(QApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier))
    {

         axisRect()->setRangeZoom(Qt::Vertical);
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
            title->setText(newTitle);
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

