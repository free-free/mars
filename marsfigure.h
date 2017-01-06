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
class MarsPlot;

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
    void getImportFileName(const QString &fileName);
    void getExportFileName(const QString &fileName);


public slots:
    void showExportDataDialog();
    void showImportDataDialog();
    void saveGraph();

    void readTextFile(QFile * file);
    void writeTextFile(QFile *file);
    void readXMLFile(QFile * file);
    void writeXMLFile(QFile * file);
    void readJSONFile(QFile * file);
    void writeJSONFile(QFile* file);
    void readDatFile(QFile * file);
    void writeDatFile(QFile * file);

    void plot(QList<QByteArray> &data);
    void plot(QByteArray &data);
    void plot(QString & data);
    void plot(double x, double y, int graphId=0, int plotId=0);
    void plot(QVector<double>& x, QVector<double> &y, int graphId,int plotId);
    void plot(QVector<double> &x, QVector<double> &y, int graphId);
    void startPlot();
    void stopPlot();
    void clearGraph();
    void onPlotActionTriggered();
    void changeCurrentPloter(int index);
    QCustomPlot* createPloter();
    void deletePloter();

private:

    void initToolBar();
    void initPlot();
    void arrangePloter();
    QColor graphColor(int graphId);
    void updateStatusBar();
    void readFile(const QString &fileName);
    void writeFile(const QString &fileName);




private:
    const int MAX_PLOTER_NUMBER = 4;
    const int MIN_PLOTER_NUMBER = 1;
    QVBoxLayout * layout;
    QGridLayout * plotLayout;
    QToolBar * toolBar;
    QAction * exportDataAction;
    QAction * importDataAction;
    QAction * plotAction;
    QAction * saveGraphAction;
    QAction * clearGraphAction;
    QAction * createPloterAction;
    QAction * deletePloterAction;
    QComboBox * ploterNameListBox;

    QCustomPlot * currentPloter;
    QList<QCustomPlot*> *ploterContainer;
    /*
     * plotState=true (start plot)
     * plotState=false (stop plot)
     */
    bool plotState;

};

#endif // MARSFIGURE_H
