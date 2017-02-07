/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: main.cpp
 *@description: this file contains the main  function
 *@version: 0.1
 *@author: jell
 *@date: 2017/02/07
 *
 *
 */

#include "mars.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mars w;
    w.show();

    return a.exec();
}
