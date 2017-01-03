#ifndef MARSFIGURE_H
#define MARSFIGURE_H

#include <QWidget>
#include "marstypes.h"

using namespace std;

class QAction;
class QToolBar;
class QGridLayout;
class QFile;
class QCustomPlot;
class QColor;
class QComboBox;
class QVBoxLayout;

class MarsFigure : public QWidget
{
    Q_OBJECT
public:
    explicit MarsFigure(QWidget *parent = 0);
    ~MarsFigure();
    MarsError  errorInstance(QString msg, MarsErrorLevel level);


signals:
    void error(MarsError error);
    void plotStarted();
    void plotStopped();

public slots:
    void exportData();
    void importData();
    void saveGraph();

    void readTextFile(QFile * file);
    void writeTextFile(QFile *file);
    void readXMLFile(QFile * file);
    void writeXMLFile(QFile * file);
    void readJSONFile(QFile * file);
    void writeJSONFile(QFile* file);

    void plot(QList<QByteArray> &data);
    void plot(QByteArray &data);
    void plot(QString & data);
    void plot(int graphId, double x, double y);
    void plot(int graphId, QVector<double>& x,QVector<double> &y);
    void startPlot();
    void stopPlot();
    void clearGraph();
    void onPlotActionTriggered();
    void changeCurrentPloter(int index);

private:

    void initToolBar();
    void initPlot();
    QColor graphColor(int graphId);
    QCustomPlot* createPloter();

private:
    const int MAX_PLOTER_NUMBER = 4;
    QVBoxLayout * layout;
    QGridLayout * plotLayout;
    QToolBar * toolBar;
    QAction * exportDataAction;
    QAction * importDataAction;
    QAction * plotAction;
    QAction * saveGraphAction;
    QAction * clearGraphAction;
    QComboBox * plotNameListBox;
    QCustomPlot * currentPloter;
    QList<QCustomPlot*> *ploterContainer;
    // plotState=true (start plot)
    // plotState=false (stop plot)
    bool plotState;

};

#endif // MARSFIGURE_H
