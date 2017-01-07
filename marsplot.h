#ifndef MARSPLOT_H
#define MARSPLOT_H

#include <QWidget>
#include "qcustomplot.h"

class MarsPlot : public QCustomPlot
{

public:
    explicit MarsPlot(QWidget * parent=0);
    ~MarsPlot();
   void wheelEvent (QWheelEvent *event);
   void focusInEvent(QFocusEvent*event);

signals:
   void focusIn(MarsPlot * focusInObj);


};

#endif // MARSPLOT_H
