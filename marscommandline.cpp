/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: marscommandline.cpp
 *@description: this file contains the implementation of command line class
 *@version: 0.1
 *@author: jell
 *@date: 2017/02/07
 *
 *
 */
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
#include <QScrollBar>
#include <QTextBlock>

#include "marscommandline.h"
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


MarsCommandLine::MarsCommandLine(QWidget * parent, bool viewOnly,int maxIBufferSize, int maxOBufferSize):QPlainTextEdit(parent)
{


    cmdStyle = new MarsCommandLine::Style(this);
    if(viewOnly)
        setReadOnly(true);
    cursorNextLine();
    setContextMenuPolicy(Qt::CustomContextMenu);
    out = new MarsBytesListBuffer(maxOBufferSize);
    in = new MarsBytesListBuffer(maxIBufferSize);
    scrollBar = new QScrollBar(this);
    scrollBar->setStyleSheet(" QScrollBar:vertical {"
                                 "border: 0px 0px 0px px;"
                                 "background: #666;"
                                 "width: 10px;"
                                 "padding:16px 0px 16px 0px;"
                             "}"
                             "QScrollBar::handle:vertical {"
                                 "background: #bbb;"
                                 "border-radius:5px;"
                                 "min-height: 60px;"
                             "}"
                             "QScrollBar::add-line:vertical {"
                                 "border: 0px 0px 0px 0px;"
                                 "border-top:1px solid #333;"
                                 "background: #888;"
                                 "height: 15px;"
                                 "subcontrol-position: bottom;"
                                 "subcontrol-origin: margin;"
                             "}"
                             "QScrollBar::sub-line:vertical {"
                                 "border: 0px 0px 0px 0px;"
                                 "border-bottom:1px solid #333;"
                                 "background: #888;"
                                 "height: 15px;"
                                 "subcontrol-position: top;"
                                 "subcontrol-origin: margin;"
                             "}"
                             );
    setVerticalScrollBar(scrollBar);
    setWordWrapMode(QTextOption::WrapAnywhere);
    connect(this,&MarsCommandLine::customContextMenuRequested,this,&MarsCommandLine::createCustomContextMenu);

}

void MarsCommandLine::createCustomContextMenu(QPoint pos)
{
    QMenu *contextMenu = new QMenu(this);
    QAction *copyAction = new QAction(tr("复    制"),contextMenu);
    QAction *pasteAction = new QAction(tr("粘    贴"),contextMenu);
    QAction *selectAllAction = new QAction(tr("全    选"),contextMenu);
    QAction *clearAction = new QAction(tr("清    除"),contextMenu);
    QAction * saveAsAction = new QAction(tr("导    出"),contextMenu);
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
    connect(saveAsAction,&QAction::triggered,this,&MarsCommandLine::exportData);
    connect(loadInAction,&QAction::triggered,this,&MarsCommandLine::importData);
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
    if(data.isEmpty())
    {
        if(!isReadOnly())
            cursorNextLine();
        return ;
    }
    insertPlainText(data);
    cursorNextLine();
    out->append(data);
    scrollBar->setValue(this->blockCount());
}

/**
 *@Desc: put data into  command line and show off
 *@Args: QString
 *@Returns: None
 */
void MarsCommandLine::printData(QString data)
{

    if(data.isEmpty())
    {
        if(!isReadOnly())
            cursorNextLine();
        return ;
    }
    insertPlainText(data);
    cursorNextLine();
    out->append(data);
    scrollBar->setValue(this->blockCount());

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
    if(!isReadOnly()&&!data.isEmpty())
    {
        in->append(data);
        emit dataIn();
    }
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
    if(!isReadOnly()&&!data.isEmpty())
    {
        in->append(data);
        emit dataIn();
    }
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
    if(!isReadOnly()&&!out->isEmpty())
    {
        in->append(out->last());
        emit dataIn();
    }
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
    if(!isReadOnly()&&!out->isEmpty())
    {
        in->append(out->last());
        emit dataIn();
    }
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


/*
 *@Desc: catch keyboard pressed key and append key value to buffer
 *@Args: QKeyEvent * e
 *@Returns: None
 */
void MarsCommandLine::keyPressEvent(QKeyEvent *e)
{

    static QByteArray inputKeyBuffer;
    if(isReadOnly())
    {
        return ;
    }
    //currentRowNumber = textCursor().blockNumber();
    //totalRowCount = blockCount();
    //qDebug()<<"block number:"<<textCursor().blockNumber();
    //qDebug()<<"block count:"<<blockCount();
    //qDebug()<<"line count:"<<document()->lineCount();
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
                if(textCursor().positionInBlock()>style()->promptLength())
                    QPlainTextEdit::keyPressEvent(e);
                break;
            /* prevent backspace key from deleting  prompt */
            case Qt::Key_Backspace:
                if(textCursor().positionInBlock()>style()->promptLength())
                {
                    QPlainTextEdit::keyPressEvent(e);
                    if(!inputKeyBuffer.isEmpty())
                        inputKeyBuffer.remove(inputKeyBuffer.length()-1,1);
                }
                break;
            /* append key value  to inputBuffer */
            default:
                inputKeyBuffer.append(e->text());
                QPlainTextEdit::keyPressEvent(e);
                break;
        }
    }

}


/*
 *@Desc: catch mouse event,
 *@Args: QMouseEvent * e
 *@Returns: None
 */
void MarsCommandLine::mousePressEvent(QMouseEvent *e)
{
    /* i have a little sad */
    setFocus();
    if(isReadOnly())
    {
        /*
         * when marscommandline is readonly,
         * just catch mouse right button pressed event and delivery to parent class
         *
         */
        if(e->button()==Qt::RightButton)
        {
            QPlainTextEdit::mousePressEvent(e);
        }
        else
        {
            Q_UNUSED(e);
        }
    }
    else
    {
        QPlainTextEdit::mousePressEvent(e);
    }
}


/*
 *@Desc: mouse double click event
 *@Args: QMouseEvent * e
 *@Returns: None
 */
void MarsCommandLine::mouseDoubleClickEvent(QMouseEvent *e)
{
    /* same ,Again!!!!!!!!!!*/
    if(isReadOnly())
    {
        /*
         * when marscommandline is readonly,
         * just catch mouse double click event and delivery to parent class
         *
         */
        if(e->button()==Qt::RightButton)
        {
            QPlainTextEdit::mousePressEvent(e);
        }
        else
        {
            Q_UNUSED(e);
        }
    }
    else
    {
        QPlainTextEdit::mousePressEvent(e);
    }
}


/*
 *@Desc: context menu event , no implementation at this time
 *@Args: QContextMenuEvent * e
 *@Returns: None
 */
void MarsCommandLine::contextMenuEvent(QContextMenuEvent *e)
{
    /*  no implementation */
    Q_UNUSED(e);
}


/**
 *@Desc: emit dataExportRequest signal
 *@Args: None
 *@Returns: None
 */
void MarsCommandLine::exportData()
{
    emit dataExportRequest();
}

/**
 *@Desc: emit dataImportRequest signal
 *@Args: None
 *@Returns: None
 */
void MarsCommandLine::importData()
{
   emit dataImportRequest();
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


/*
 *@Desc: emit focusIn signal
 *@Args: QFocusEvent * event
 *@Returns: None
 */
void MarsCommandLine::focusInEvent(QFocusEvent *event)
{

    if(event->gotFocus())
    {
        emit focusIn(this);
    }
}


/*
 *@Desc: no implementation at this time
 *@Args: QFocusEvent * e
 *@Returns: None
 */
void MarsCommandLine::focusOutEvent(QFocusEvent *event)
{
    /*
     * nothing here
     */
    Q_UNUSED(event);
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


/*
 *@Desc: set command line's cursor width
 *@Args: int width
 *@Returns: None
 */
void MarsCommandLine::Style::cursorWidth(int w)
{
    cursorWid = w;
    parent->setCursorWidth(cursorWid);
}


/*
 *@Desc: return command line's cursor width
 *@Args: None
 *@Returns: int
 */
int MarsCommandLine::Style::cursorWidth() const
{
    return cursorWid;
}


/*
 *@Desc: return command line's prompt string length
 *@Args: None
 *@Returns: int
 */
int MarsCommandLine::Style::promptLength() const
{
    return promptStringLength;
}


/*
 *@Desc: set stylesheet for command line
 *@Args: const QString & stylesheet
 *@Returns: None
 */
void MarsCommandLine::Style::setStyleSheet(const QString &sh)
{
    styleSheet = sh;
    parent->setStyleSheet(styleSheet);
}


/*
 *@Desc: add stylesheet for command line
 *@Args: const QSting & stylesheet
 *@Returns: None
 */
void MarsCommandLine::Style::addStyleSheet(const QString &sh)
{
    styleSheet +=sh;
    parent->setStyleSheet(styleSheet);
}


/*
 *@Desc: return command line's style container
 *@Args: None
 *@Returns: MarsCommandLine::Style *
 */
MarsCommandLine::Style * MarsCommandLine::style()
{
    return cmdStyle;
}
