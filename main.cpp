#include "mars.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mars w;
    w.show();

    return a.exec();
}
