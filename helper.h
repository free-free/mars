/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: helper.h
 *@description: this file contains the declaraction of helper  function
 *              and some useful data structure.
 *@version: 0.1
 *@author: jell
 *@date: 2017/01/12
 *
 *
 */
#ifndef HELPER_H
#define HELPER_H

#include <QByteArray>


/*
 *@Desc: a union for converting bytes to float
 *@Memebers: float, char [4]
 *
 */
union BytesToFloatUnion
{
    float fnumber;
    char bytes[4];
};

/*
 *@Desc: a union for converting bytes to double
 *@Members: double , char [8]
 */
union BytesToDoubleUnion
{
    double dnumber;
    char bytes[8];
};

float bytesToFloat(QByteArray bytes);
double bytesToDouble(QByteArray bytes);

#endif // HELPER_H
