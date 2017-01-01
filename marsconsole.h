#ifndef MARSCONSOLE_H
#define MARSCONSOLE_H

#include <QWidget>
#include "marstypes.h"

class MarsCommandLine;
class QFile;
class QGridLayout;
class QList<MarsCommandLine>;
class QToolBar;

class MarsConsole : public QWidget
{
    Q_OBJECT
public:
    explicit MarsConsole(QWidget *parent = 0,bool view_only=false,int maxIBufferSize=500,int maxOBufferSize=1000);
    ~MarsConsole();
    MarsCommandLine & commandLine(int index = 0);

signals:
    void error(MarsError  error);
    void dataReady();


public slots:
    void saveDataAs();
    void loadDataIn();
    void readTextFile(QFile * file);
    void writeTextFile(QFile *file);
    void readXMLFile(QFile * file);
    void writeXMLFile(QFile * file);
    void readJSONFile(QFile * file);
    void writeJSONFile(QFile* file);
    MarsError  errorInstance(QString msg, MarsErrorLevel level);
    void onCommandLineDataReady();
    bool createCommandLine(int number,bool view_only=false,int maxIBufferSize=500, int maxOBufferSize=1000);
    void createToolBar();
private:
    QList<MarsCommandLine*> commandLines;
    QGridLayout * layout;
    QToolBar * toolBar;
    QAction * saveAsAction;
    QAction * loadInAction;

};

#endif // MARSCONSOLE_H
