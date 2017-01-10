#ifndef MARSCommandLine_H
#define MARSCommandLine_H

#include <QPlainTextEdit>
#include "marstypes.h"

using namespace std;

class QKeyEvent;
class QMouseEvent;
class QContextMenuEvent ;
class QColor;
class QTextStream;
class MarsBytesListBuffer;
class QFocusEvent;
class QScrollBar;

class MarsCommandLine : public QPlainTextEdit
{

    Q_OBJECT
public:
    // console style inner class
    class Style
    {
            QString promptString;
            int promptStringLength;
            int cursorWid;
            QColor tColor;
            QColor bgColor;
            QString styleSheet;
            MarsCommandLine * parent;
        public:
            Style(MarsCommandLine *parent);
            void prompt(QString);
            QString prompt() const;
            void textColor(QColor color);
            QColor textColor() const;
            void baseColor(QColor color);
            QColor baseColor() const;
            void cursorWidth(int w);
            int cursorWidth() const ;
            int promptLength() const;
            void setStyleSheet(const QString &stylesheet);
            void addStyleSheet(const QString &stylesheet);

    };

public:
    explicit MarsCommandLine(QWidget * parent = 0, bool viewOnly=false,int maxIBufferSize=500,int maxOBufferSize=1000);
    ~MarsCommandLine();
    void printData(QByteArray data);
    void printData(QString data);
    QByteArray readData();
    void enableViewOnly(bool ro);
    void cursorNextLine(const QString &ps);
    void cursorNextLine();
    MarsError errorInstance(QString msg, MarsErrorLevel=INFO);
    MarsCommandLine::Style * style();
    MarsCommandLine & operator <<(const QByteArray &data);
    MarsCommandLine & operator >>(QByteArray & data);
    MarsCommandLine & operator <<(const QString &data);
    MarsCommandLine & operator >>(QString & data);
    MarsCommandLine & operator <<(MarsCommandLine & dataSender);
    MarsCommandLine & operator >>(MarsCommandLine & dataReceiver);
    MarsCommandLine & operator <<(QTextStream & dataSender);
    MarsCommandLine & operator >>(QTextStream & dataReceiver);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

    // buffer operation
    MarsBytesListBuffer * inputBuffer() ;
    MarsBytesListBuffer * outputBuffer();
    QString  inputBufferText();
    QString  outputBufferText();


public slots:
    void createCustomContextMenu(QPoint pos);
    void clearAll();
    void exportData();
    void importData();



signals:
    void dataIn();
    void errors(MarsError  error );
    void dataImportRequest();
    void dataExportRequest();
    void focusIn(MarsCommandLine * focusInObj);

private:
    Style * cmdStyle;
    // input and output buffer;
    MarsBytesListBuffer *in;
    MarsBytesListBuffer *out;
    QScrollBar * scrollBar;


protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e );
    virtual void mouseDoubleClickEvent(QMouseEvent * e);
    virtual void contextMenuEvent(QContextMenuEvent * e);

};

#endif // MarsCommandLine_H
