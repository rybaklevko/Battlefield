#ifndef CPLAYER_H
#define CPLAYER_H

#include <QTcpSocket>
#include <QObject>
#include <QString>
#include "constants.h"

struct CPlayer
{
    CPlayer();
    bool sendData(const QString &data);
    QTcpSocket *mSocket;
};

#endif // CPLAYER_H
