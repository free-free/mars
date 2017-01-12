#ifndef MARSBYTESQUEUE_H
#define MARSBYTESQUEUE_H


#include <QQueue>
class MarsBytesQueue : public QQueue<QByteArray>
{
public:
    MarsBytesQueue(int maxSize=-1);
    ~MarsBytesQueue();
    QByteArray dequeue();
    void enqueue(const QByteArray &t);
private:
    int maxQueueSize;
};

#endif // MARSBYTESQUEUE_H
