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
<<<<<<< HEAD
class MarsPlot;
=======
>>>>>>> 7c783581f0b0f261bf033746213b2068f17038de

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
<<<<<<< HEAD
    void getImportFileName(const QString &fileName);
    void getExportFileName(const QString &fileName);


public slots:
    void showExportDataDialog();
    void showImportDataDialog();
=======

public slots:
    void exportData();
    void importData();
>>>>>>> 7c783581f0b0f261bf033746213b2068f17038de
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
<<<<<<< HEAD
    void plot(double x, double y, int graphId=0, int plotId=0);
    void plot(QVector<double>& x, QVector<double> &y, int graphId=0,int plotId=0);
=======
    void plot(int graphId, double x, double y);
    void plot(int graphId, QVector<double>& x,QVector<double> &y);
>>>>>>> 7c783581f0b0f261bf033746213b2068f17038de
    void startPlot();
    void stopPlot();
    void clearGraph();
    void onPlotActionTriggered();
    void changeCurrentPloter(int index);
<<<<<<< HEAD
    QCustomPlot* createPloter();
    void deletePloter();
=======
>>>>>>> 7c783581f0b0f261bf033746213b2068f17038de

private:

    void initToolBar();
    void initPlot();
<<<<<<< HEAD
    void arrangePloter();
    QColor graphColor(int graphId);
    void updateStatusBar();
    void readFile(const QString &fileName);
    void writeFile(const QString &fileName);




private:
    const int MAX_PLOTER_NUMBER = 4;
    const int MIN_PLOTER_NUMBER = 1;
=======
    QColor graphColor(int graphId);
    QCustomPlot* createPloter();

private:
    const int MAX_PLOTER_NUMBER = 4;
>>>>>>> 7c783581f0b0f261bf033746213b2068f17038de
    QVBoxLayout * layout;
    QGridLayout * plotLayout;
    QToolBar * toolBar;
    QAction * exportDataAction;
    QAction * importDataAction;
    QAction * plotAction;
    QAction * saveGraphAction;
    QAction * clearGraphAction;
<<<<<<< HEAD
    QAction * createPloterAction;
    QAction * deletePloterAction;
    QComboBox * ploterNameListBox;

    QCustomPlot * currentPloter;
    QList<QCustomPlot*> *ploterContainer;
    /*
     * plotState=true (start plot)
     * plotState=false (stop plot)
     */
=======
    QComboBox * plotNameListBox;
    QCustomPlot * currentPloter;
    QList<QCustomPlot*> *ploterContainer;
    // plotState=true (start plot)
    // plotState=false (stop plot)
>>>>>>> 7c783581f0b0f261bf033746213b2068f17038de
    bool plotState;

};

#endif // MARSFIGURE_H
