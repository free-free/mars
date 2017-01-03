#include "marsplot.h"
#include "qcustomplot.h"
#include <QWheelEvent>

MarsPlot::MarsPlot(QWidget * parent ):QCustomPlot(parent)
{
    /* nothing is here now */
}


MarsPlot::~MarsPlot()
{
    /* nothing is here now */
}


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
    qDebug()<<"mars plot's wheel event be called";
}
