#ifndef CUSER_H
#define CUSER_H

#include <QTcpSocket>
#include <QMap>
#include "cfield.h"


//structure,which holds information about connected user
struct CUser
{
    void sendData(const QString &data);
    QTcpSocket *mTcpSocket;
    CField mField;
    ECurrentClientState mState;
    QMap<int,CUser> ::iterator mEnemyPlayer;
    int mNumberOfMarkedCells = 0;
};

#endif // CUSER_H
