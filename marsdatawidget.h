#ifndef MARSDATAWIDGET_H
#define MARSDATAWIDGET_H

#include <QWidget>
#include "marstypes.h"

using namespace std;

class QFile;
class QAction;
class QToolBar;
class QGridLayout;


class MarsDataWidget:public QWidget
{
    Q_OBJECT
public:
    explicit MarsDataWidget(QWidget * parent=0);
    ~MarsDataWidget();

signals:
    void error(MarsError  error);
public slots:
    void exportData();
    void importData();
    QToolBar & toolBar();
    MarsError errorInstance(QString msg, MarsErrorLevel level=INFO);

    virtual void readTextFile(QFile * file);
    virtual void writeTextFile(QFile * file);
    virtual void readJSONFile(QFile * file);
    virtual void writeJSONFile(QFile * file);
    virtual void readXMLFile(QFile * file);
    virtual void writeXMLFile(QFile * file);
private:
    void createToolBar();

private:
    QGridLayout * layout;
    QToolBar * toolBarIns;

};

#endif // MARSDATAWIDGET_H
