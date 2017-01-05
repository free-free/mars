#ifndef MARSBYTESLISTBUFFER_H
#define MARSBYTESLISTBUFFER_H

#include <QList>

class QTextStream;
class MarsBytesListBuffer : public QList<QByteArray>
{

public:
    MarsBytesListBuffer(int maxsize=500);
    ~MarsBytesListBuffer();
private:
    int maxSize;
    int updatedFlag;
public:
    void append( const QByteArray &t);
    void append(const QString &t);
    bool isUpdated();
    void setUpdatedFlag(bool update);
    void setMaxSize(int maxsize);
    MarsBytesListBuffer flush();
    MarsBytesListBuffer & operator<<(const QString &t);
    MarsBytesListBuffer & operator<<(const QByteArray &t);
    MarsBytesListBuffer & operator<<(const MarsBytesListBuffer &t);
};

#endif // MARSBYTESLISTBUFFER_H
