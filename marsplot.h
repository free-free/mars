#ifndef MARSPLOT_H
#define MARSPLOT_H

#include <QWidget>
#include "qcustomplot.h"

class MarsPlot : public QCustomPlot
{

public:
    MarsPlot(QWidget * parent=0);
    ~MarsPlot();
   void wheelEvent (QWheelEvent *event);

};

#endif // MARSPLOT_H
