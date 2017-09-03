#include "cplayer.h"

//constructor,initialize socket and make connection
CPlayer::CPlayer()
{
    mSocket = new QTcpSocket();
    mSocket->connectToHost(kDefaultAddress,kDefaultPortNumber);

    if(mSocket->waitForConnected(kDefaultTimeValue)) {
        qDebug()<<"Connected!";
    } else {
        qDebug()<<"Wasn't connected!";
        mSocket->close();
    }
}

//function,which send data
bool CPlayer::sendData(const QString &data)
{
    mSocket->write(data.toUtf8());

}



