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
