/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: marsbyteslistbuffer.cpp
 *@description: this file contains the sized byte list buffer stucture imlementation
 *@version: 0.1
 *@author: jell
 *@date: 2017/02/07
 *
 *
 */
#include "marsbyteslistbuffer.h"
#include <QTextStream>
#include <QDebug>

MarsBytesListBuffer::MarsBytesListBuffer(int maxsize ):QList<QByteArray>()
{
    maxSize = maxsize;
    updatedFlag = false;
}

MarsBytesListBuffer::~MarsBytesListBuffer()
{

}

void MarsBytesListBuffer::append( const QByteArray &t)
{
    if(QList<QByteArray>::size()>=maxSize)
        QList<QByteArray>::pop_front();
    QList<QByteArray>::append(t);
    if(!t.isEmpty())
    {
        updatedFlag = true;
    }

}

void MarsBytesListBuffer::append(const QString &t)
{
    if(QList<QByteArray>::size()>=maxSize)
        QList<QByteArray>::pop_front();
    QList<QByteArray>::append(t.toUtf8());
    if(!t.isEmpty())
    {
        updatedFlag  = true;
    }
}

bool MarsBytesListBuffer::isUpdated()
{
    return updatedFlag;
}

void MarsBytesListBuffer::setUpdatedFlag(bool update)
{
    updatedFlag = update;
}

void MarsBytesListBuffer::setMaxSize(int maxsize)
{
    maxSize = maxsize;
}

MarsBytesListBuffer & MarsBytesListBuffer::operator <<(const QString &t)
{
    append(t);
    return *this;
}

MarsBytesListBuffer & MarsBytesListBuffer::operator <<(const QByteArray &t)
{
    append(t);
    return *this;
}

MarsBytesListBuffer & MarsBytesListBuffer::operator <<(const MarsBytesListBuffer &t)
{
    if(!t.isEmpty())
    {
        QList<QByteArray>::append(t);
        updatedFlag = true;
    }
    return *this;
}

MarsBytesListBuffer MarsBytesListBuffer::flush()
{
    MarsBytesListBuffer buffer;
    buffer = *this;
    this->clear();
    return buffer;
}
