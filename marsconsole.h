#ifndef MARSCONSOLE_H
#define MARSCONSOLE_H

#include <QWidget>
#include "marstypes.h"

class MarsCommandLine;
class QFile;
class QGridLayout;
class QList<MarsCommandLine>;
class QToolBar;
class QComboBox;
class QVBoxLayout;
class QFocusEvent;

class MarsConsole : public QWidget
{
    Q_OBJECT
public:
    explicit MarsConsole(QWidget *parent = 0,bool view_only=false,int maxIBufferSize=500,int maxOBufferSize=1000);
    ~MarsConsole();
    MarsCommandLine * commandLine(int index = 0);
    MarsError  errorInstance(QString msg, MarsErrorLevel level);

signals:
    void errors(MarsError  error);
    void dataReady();
    void getImportFileName(QString fileName);
    void getExportFileName(QString fileName);
    void plotDataRequest(MarsCommandLine * requestObj);

public slots:
    void showExportDataDialog();
    void showImportDataDialog();
    void readFile(QString fileName);
    void writeFile(QString fileName);
    void readTextFile(QFile * file);
    void writeTextFile(QFile *file);
    void readJSONFile(QFile * file);
    void writeJSONFile(QFile* file);
    void readDatFile(QFile * file);
    void writeDatFile(QFile * file);
    void deleteCurrentCmdLine();
    void deleteCmdLine(int index);
    void changeCurrentCmdLine(int index);
    MarsCommandLine* createCmdLine(bool viewOnly=false,int maxIBufferSize=500, int maxOBufferSize=1000);
    void clearCurrentCmdLine();
    void onCmdLineDataReady();
    void changeCurrentCmdLineOnFocusIn(MarsCommandLine * focusInObj);
    void onPlotDataActionTriggered();



private:
    void updateStatusBar();
    void initToolBar();
    void initCmdLine(bool viewOnly,int maxIBufferSize,int maxOBufferSize);
    void arrangeCommandLine();

private:
    const int MAX_CMDLINE_NUMBER=4;
    QList<MarsCommandLine*> *cmdLineContainer;
    MarsCommandLine * currentCmdLine;
    QGridLayout * cmdLineLayout;
    QVBoxLayout * layout;
    QToolBar * toolBar;
    QAction * exportDataAction;
    QAction * importDataAction;
    QAction * clearCmdLineAction;
    QAction * plotDataAction;
    QComboBox * cmdLineNameListBox;

};

#endif // MARSCONSOLE_H
