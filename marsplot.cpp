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

    }
}
