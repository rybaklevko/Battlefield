#include "cuser.h"

//function,to send data
void CUser::sendData(const QString &data)
{
    mTcpSocket->write(data.toUtf8());
    mTcpSocket->flush();
    mTcpSocket->waitForBytesWritten(kDefaultTimeValue);
}

