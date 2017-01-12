/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: helper.cpp
 *@description: this file contains the definition of helper  function
 *@version: 0.1
 *@author: jell
 *@date: 2017/01/12
 *
 *
 */

#include "helper.h"


/*
 *@Desc: convert 8 bytes array to double number
 *@Args: QByteArray bytes(8 byte)
 *@Returns: double
 *
 */
double bytesToDouble(QByteArray bytes)
{
    BytesToDoubleUnion converter;
    // bytes length must be 8
    if(bytes.length() != 8)
        return 0.0;
    for ( int i = 0 ; i < 8; i++)
    {
        converter.bytes[i] = bytes.at(i);
    }
    return converter.dnumber;
}
/*
 *@Desc: convert 4 bytes array to float number
 *@Args: QByteArray bytes(4 byte)
 *@Returns: float
 *
 */
float bytesToFloat(QByteArray bytes)
{
    BytesToFloatUnion converter;
    // bytes length must be 4
    if(bytes.length() != 4)
        return 0.0;
    for( int i = 0 ; i < 4; i++)
    {
        converter.bytes[i] = bytes.at(i);
    }
    return converter.fnumber;
}
