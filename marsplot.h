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
   double xMaxRange() const;
   double yMaxRange() const;

protected slots:
   void changeGraphLegendName(QCPLegend *legend, QCPAbstractLegendItem *item);
   void changeAxisLabelName(QCPAxis * axis,QCPAxis::SelectablePart);
   void changeTitle(QMouseEvent * event);
   void onMouseMove(QMouseEvent * event);

signals:
   void focusIn(MarsPlot * focusInObj);

private:
   QString titleText;
   double xMaxRangeValue;
   double yMaxRangeValue;

};

#endif // MARSPLOT_H
