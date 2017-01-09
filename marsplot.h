#ifndef MARSPLOT_H
#define MARSPLOT_H

#include <QWidget>
#include "qcustomplot.h"

class MarsPlot : public QCustomPlot
{

Q_OBJECT
public:
    explicit MarsPlot(QWidget * parent=0);
    ~MarsPlot();
   void wheelEvent (QWheelEvent *event);
   void focusInEvent(QFocusEvent*event);

protected slots:
   void changeGraphLegendName(QCPLegend *legend, QCPAbstractLegendItem *item);
   void changeAxisLabelName(QCPAxis * axis,QCPAxis::SelectablePart);
   void changeTitle(QMouseEvent * event);

signals:
   void focusIn(MarsPlot * focusInObj);


};

#endif // MARSPLOT_H
