#include "marscommandline.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QColor>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QFileDialog>
#include <QTextStream>
#include "marsbyteslistbuffer.h"



QTextStream & operator<<(QTextStream & out, QList<QByteArray> &list)
{
    QStringList outputStrList;
    for(int i=0;i<list.size();++i)
    {
        outputStrList<<list.at(i);
    }
    out<<outputStrList.join("\n");
    return out;
}

QTextStream & operator<<(QTextStream & out,QList<QByteArray> * list)
{
    QStringList outputStrList;
    for(int i=0;i<list->size();++i)
    {
        outputStrList<<list->at(i);
    }
    out<<outputStrList.join("\n");
    return out;
}

QTextStream & operator>>(QTextStream & in,QList<QByteArray> &list)
{
    QString inputStr;
    inputStr = in.readAll();
    QStringList inputStrList;
    inputStrList = inputStr.split(QRegExp("[\n\r,;]+"));
    for(int i=0;i<inputStrList.size();++i)
    {
        list.append(inputStrList.at(i).toUtf8());
    }
    return in;
}

QTextStream & operator>>(QTextStream & in,QList<QByteArray> *list)
{
    QString inputStr;
    inputStr = in.readAll();
    QStringList inputStrList;
    inputStrList = inputStr.split(QRegExp("[\n\r,;]+"));
    for(int i=0;i<inputStrList.size();++i)
    {
        list->append(inputStrList.at(i).toUtf8());
    }
    return in;
}


MarsCommandLine::MarsCommandLine(QWidget * parent, bool view_only,int maxIBufferSize, int maxOBufferSize):QPlainTextEdit(parent)
{
    document()->setMaximumBlockCount(100);
    cmdStyle = new MarsCommandLine::Style(this);
    if(view_only)
        setReadOnly(true);
    else
        cursorNextLine();
    setContextMenuPolicy(Qt::CustomContextMenu);
    out = new MarsBytesListBuffer(maxOBufferSize);
    in = new MarsBytesListBuffer(maxIBufferSize);
    connect(this,&MarsCommandLine::customContextMenuRequested,this,&MarsCommandLine::createCustomContextMenu);
}

void MarsCommandLine::createCustomContextMenu(QPoint pos)
{
    QMenu *contextMenu = new QMenu(this);
    QAction *copyAction = new QAction(tr("复    制"),contextMenu);
    QAction *pasteAction = new QAction(tr("粘    贴"),contextMenu);
    QAction *selectAllAction = new QAction(tr("全    选"),contextMenu);
    QAction *clearAction = new QAction(tr("清    除"),contextMenu);
    QAction * saveAsAction = new QAction(tr("另  存  为"),contextMenu);
    QAction * loadInAction = new QAction(tr("导    入"),contextMenu);
    contextMenu->setStyleSheet(
                "QMenu{"
                "    background-color:#555;"
                "    border:0px 0px 0px 0px;"
                "    color:#f1f1f1;"
                "}"
                "QMenu::item{"
                "    padding:3px 50px;"
                "    margin:0px 0px 0px 0px;"
                "    font-weight:400;"
                "}"
                "QMenu::item:tearoff{"
                "    color:#efefef;"
                "}"
                "QMenu::separator {"
                 "   height: 1px;"
                "}"
                "QMenu::item:selected{"
                "    background-color:#ff9900;"
                "}"
                "QMenu::item::pressed{"
                    "background-color:#ff9900;"
                "}"
                );
    contextMenu->addAction(copyAction);
    contextMenu->addSeparator();
    contextMenu->addAction(pasteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(selectAllAction);
    contextMenu->addSeparator();
    contextMenu->addAction(clearAction);
    contextMenu->addSeparator();
    contextMenu->addAction(loadInAction);
    contextMenu->addSeparator();
    contextMenu->addAction(saveAsAction);
    if(!textCursor().hasSelection())
        copyAction->setVisible(false);
    if(!canPaste())
        pasteAction->setVisible(false);
    if(isReadOnly())
        loadInAction->setVisible(false);
    connect(copyAction,&QAction::triggered,this,&MarsCommandLine::copy);
    connect(pasteAction,&QAction::triggered,this,&MarsCommandLine::paste);
    connect(clearAction,&QAction::triggered,this,&MarsCommandLine::clearAll);
    connect(selectAllAction,&QAction::triggered,this,&MarsCommandLine::selectAll);
    connect(saveAsAction,&QAction::triggered,this,&MarsCommandLine::saveData);
    connect(loadInAction,&QAction::triggered,this,&MarsCommandLine::loadData);
    contextMenu->popup(viewport()->mapToGlobal(pos));
}
MarsCommandLine::~MarsCommandLine()
{
    delete in;
    delete out;
    delete cmdStyle;
}

/**
 *@Desc: put data into  command line and show off
 *@Args: QByteArray
 *@Returns: None
 */

void MarsCommandLine::printData(QByteArray  data)
{
    /* please implement me ,bitch*/
    cursorNextLine();
    insertPlainText(data);
    out->append(data);

}

/**
 *@Desc: put data into  command line and show off
 *@Args: QString
 *@Returns: None
 */
void MarsCommandLine::printData(QString data)
{

    cursorNextLine();
    insertPlainText(data);
    out->append(data);

}

/**
 *@Desc: get input data from command line
 *@Args: None
 *@Return: QByteArray
 *
 */
QByteArray MarsCommandLine::readData()
{
    /* fuck me ,sha bi */
    QByteArray data;
    if(in->isUpdated())
    {
        data = in->last();
        in->setUpdatedFlag(false);
    }
    return data;
}

/**
 *@Desc: reload command line operator <<
 *@Args: const QByteArray &
 *@Returns: MarsCommandLine &
 */
MarsCommandLine & MarsCommandLine::operator <<(const QByteArray &data)
{
    printData(data);
    in->append(data);
    emit dataIn();
    return *this;
}

/**
 *@Desc: reload command line operator <<
 *@Args: const QString &
 *@Returns: MarsCommandLine &
 */

MarsCommandLine & MarsCommandLine::operator <<(const QString &data)
{
    printData(data);
    in->append(data);
    emit dataIn();
    return *this;
}

/**
 *@Desc: reload command line operator <<
 *@Args: MarsCommandLine & (send data)
 *@Return: MarsCommandLine & (receive data)
 */
MarsCommandLine & MarsCommandLine::operator <<( MarsCommandLine &dataSender)
{

    printData(dataSender.readData());
    in->append(out->last());
    emit dataIn();
    return *this;
}
/**
 *@Desc: reload command line operator <<
 *@Args: QTextStream   (data sender)
 *@Returns: MarsCommandLine & (data receiver)
 */
MarsCommandLine & MarsCommandLine::operator <<(QTextStream & dataSender)
{
    printData(dataSender.readAll());
    in->append(out->last());
    emit dataIn();
    return *this;
}
/**
 *@Desc: reload command line operator >>
 *@Args: QByteArray &data
 *@Returns: MarsCommandLine &
 */
MarsCommandLine & MarsCommandLine::operator >>(QByteArray &data)
{
    data = readData();
    return *this;
}

/**
 *@Desc: reload command line operator >>
 *@Args: QString & data
 *@Returns: MarsCommandLine &
 */

MarsCommandLine & MarsCommandLine::operator >>(QString &data)
{
    data = readData();
    return *this;
}
/**
 *@Desc: reload command line operator >>
 *@Args: MarsCommandLine & (receive data)
 *@Returns: MarsCommandLine &(send data)
 */
MarsCommandLine & MarsCommandLine::operator >>(MarsCommandLine & dataReceiver)
{
    dataReceiver.printData(readData());
    return *this;
}

/**
 *@Desc: reload command operator >>
 *@Args: QTextStream & (data receiver)
 *@Return: MarsCommandLine &(data sender)
 */
MarsCommandLine & MarsCommandLine::operator >>(QTextStream & dataReceiver)
{
    dataReceiver<<readData();
    return *this;
}
/**
 *@Desc:  return the cursor column number that is different to textCursor's columnNumber
 *       !!!! this cursor column number
 *          is equal to  textCursor'scolumnNumber minus prompt's length;
 *@Args: None
 *@Return: int
 */
int MarsCommandLine::cursorColumnNumber() const
{
    return (textCursor().columnNumber()-cmdStyle->promptLength());
}
/**
 *@Desc: set view only mode ( or read only mode)
 *@Args: bool
 *@Returns: None
 *
 */
void MarsCommandLine::enableViewOnly(bool ro)
{
    setReadOnly(ro);
    style()->prompt("");
}

void MarsCommandLine::keyPressEvent(QKeyEvent *e)
{

    static QByteArray inputKeyBuffer;
    if(isReadOnly())
    {
        return ;
    }
    //currentRowNumber = textCursor().blockNumber();
    //totalRowCount = blockCount();
    if(textCursor().blockNumber()<(blockCount()-1))
    {
        switch(e->key())
        {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                moveCursor(QTextCursor::End);
        }
    }
    else
    {
        switch(e->key())
        {
            case Qt::Key_Up:
                break;
            /*
             * when enter or return key pressed
             * emit dataReadyRead signal and append prompt to cmd console
             *
             */
            case Qt::Key_Return:
            case Qt::Key_Enter:
                    if(!inputKeyBuffer.isEmpty())
                    {
                        *in<<inputKeyBuffer;
                        emit dataIn();
                        inputKeyBuffer.clear();
                    }
                    cursorNextLine();
                break;
            case Qt::Key_Left:
                if(cursorColumnNumber()>0)
                    QPlainTextEdit::keyPressEvent(e);
                break;
            case Qt::Key_Backspace: // prevent backspace key from deleting  prompt
                if(cursorColumnNumber()>0)
                {
                    QPlainTextEdit::keyPressEvent(e);
                    if(!inputKeyBuffer.isEmpty())
                        inputKeyBuffer.remove(cursorColumnNumber(),1);
                }
                break;
            default:
            // append key value  to inputBuffer;
                inputKeyBuffer.append(e->text());
                QPlainTextEdit::keyPressEvent(e);
                break;
        }
    }

}

void MarsCommandLine::mousePressEvent(QMouseEvent *e)
{
   // Q_UNUSED(e);
    /* i have a little sad */
    setFocus();
    QPlainTextEdit::mousePressEvent(e);

}

void MarsCommandLine::mouseDoubleClickEvent(QMouseEvent *e)
{
    /* same ,Again!!!!!!!!!!*/
    QPlainTextEdit::mouseDoubleClickEvent(e);
}

void MarsCommandLine::contextMenuEvent(QContextMenuEvent *e)
{

    /* hey ,boy */
    /* do what you can */
    Q_UNUSED(e);
}


/**
 *@Desc: emit dataSaveRequest signal
 *@Args: None
 *@Returns: None
 */
void MarsCommandLine::saveData()
{
    emit dataSaveRequest();
}

/**
 *@Desc: emit dataLoadRequest signal
 *@Args: None
 *@Returns: None
 */
void MarsCommandLine::loadData()
{
   emit dataLoadRequest();
}
/**

 *@Desc: change text cursor to next line and print prompt string
 *@Args: QString
 *@Returns: None
 */
void MarsCommandLine::cursorNextLine(const QString &ps)
{
    moveCursor(QTextCursor::End);
    appendPlainText(ps);
}

/**
 *@Desc: change text cursor to next line and print prompt string
 *@Args: None
 *@Returns: None
 */
void MarsCommandLine::cursorNextLine()
{
    moveCursor(QTextCursor::End);
    appendPlainText(style()->prompt());
}

/**
 *@Desc: return command line input buffer
 *@Args: None
 *@Returns: ConsonleBuffer *
 *
 */
MarsBytesListBuffer * MarsCommandLine::inputBuffer()
{
    return in;
}
/**
 *@Desc: return input buffer text
 *@Args: None
 *@Returns: QString &
 */
QString  MarsCommandLine::inputBufferText()
{
    QString intputStr = in->join("\n");
    return intputStr;
}


/**
 *@Desc: return command line output buffer
 *@Args: None
 *@Returns: None
 */
MarsBytesListBuffer * MarsCommandLine::outputBuffer()
{
    return out;
}
/**
 *@Desc: return output buffer text
 *@Args: None
 *@Returns: QString &
 */
QString  MarsCommandLine::outputBufferText()
{
    QString outputStr = out->join("\n");
    return outputStr;
}
/**
 *@Desc: clear command line data(including output buffer  ,but except input buffer)
 *@Args: None
 *@Return: None
 */

void MarsCommandLine::clearAll()
{
    clear();
    out->clear();
    if(!isReadOnly())
    {
        cursorNextLine();
    }
}

/**
 *@Desc: return error instance
 *@Args:  QString ,MarsErrorLevel
 *@Returns: MarsError
 */
MarsError MarsCommandLine::errorInstance(QString msg, MarsErrorLevel level)
{
    MarsError error;
    error.datetime = QDateTime::currentDateTime();
    error.type = CONSOLE;
    error.msg = msg;
    error.level = level;
    return error;
}


/* **********************************************************************
 *
 * ----------MarsCommandLine::Style class implementation--------------------------
 * --
 */

MarsCommandLine::Style::Style(MarsCommandLine * par)
{
    parent = par;
    promptString = ">>>  ";
    promptStringLength = promptString.length();
    tColor  = Qt::blue;
    bgColor = Qt::black;
    cursorWid = 10;
    styleSheet = "border:0px 0px 0px 0px;"
                 "font-weight:600;"
                 "padding:0px 0px 0px 0px;";
    QPalette p = parent->palette();
    p.setColor(QPalette::Base, bgColor);
    p.setColor(QPalette::Text, tColor);
    parent->setPalette(p);
    parent->setStyleSheet(styleSheet);
    parent->setCursorWidth(cursorWid);
}

/**
 *@Desc: return prompt string
 *@Args: None
 *Return: QString
 *
 */
QString MarsCommandLine::Style::prompt() const
{
    return promptString;
}

/**
 *@Desc: set prompt string
 *@Args: String
 *@Returns: None
 */
void MarsCommandLine::Style::prompt(QString ps)
{
    promptString = ps;
    promptStringLength = promptString.length();
}

/**
 *@Desc: set command line text color
 *@Args: QColor
 *@Returns: None
 *
 */
void MarsCommandLine::Style::textColor(QColor color)
{
    tColor = color;
    QPalette p = parent->palette();
    p.setColor(QPalette::Text,tColor);
    p.setColor(QPalette::Base,bgColor);
    parent->setPalette(p);
}

/**
 *@Desc: return command line text color
 *@Args: None
 *@Returns: QColor
 */
QColor MarsCommandLine::Style::textColor() const
{
    return tColor;
}


/**
 *@Desc: set command line background base color
 *@Args: QColor
 *@Returns: None
 *
 */
void MarsCommandLine::Style::baseColor(QColor color)
{
    bgColor = color;
    QPalette p = parent->palette();
    p.setColor(QPalette::Text,tColor);
    p.setColor(QPalette::Base,bgColor);
    parent->setPalette(p);
}


/**
 *@Desc: return command line background base color
 *@Args: None
 *@Return: QColor
 *
 */
QColor MarsCommandLine::Style::baseColor() const
{
    return bgColor;
}

void MarsCommandLine::Style::cursorWidth(int w)
{
    cursorWid = w;
    parent->setCursorWidth(cursorWid);
}

int MarsCommandLine::Style::cursorWidth() const
{
    return cursorWid;
}


int MarsCommandLine::Style::promptLength() const
{
    return promptStringLength;
}


void MarsCommandLine::Style::setStyleSheet(const QString &sh)
{
    styleSheet = sh;
    parent->setStyleSheet(styleSheet);
}

void MarsCommandLine::Style::addStyleSheet(const QString &sh)
{
    styleSheet +=sh;
    parent->setStyleSheet(styleSheet);
}


MarsCommandLine::Style * MarsCommandLine::style()
{
    return cmdStyle;
}








