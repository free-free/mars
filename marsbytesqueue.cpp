/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: marsbytequeue.cpp
 *@description: this file contains bytes queue data structure implementation
 *@version: 0.1
 *@author: jell
 *@date: 2017/02/07
 *
 *
 */
#include "marsbytesqueue.h"

#include <QDebug>

MarsBytesQueue::MarsBytesQueue(int maxSize):QQueue<QByteArray>()
{
    maxQueueSize = maxSize;
}


MarsBytesQueue::~MarsBytesQueue()
{

}

void MarsBytesQueue::enqueue(const QByteArray &t)
{

    if(maxQueueSize > 0)
    {
        QQueue<QByteArray>::enqueue(t);
        while(this->size() > maxQueueSize)
        {
            QQueue<QByteArray>::dequeue();
        }
    }
    else
    {
        QQueue<QByteArray>::enqueue(t);
    }
}

QByteArray MarsBytesQueue::dequeue()
{
    if(this->size()==0)
    {
        QByteArray data;
        return data;
    }
    return QQueue<QByteArray>::dequeue();

}
