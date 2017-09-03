#ifndef CCLIENT_H
#define CCLIENT_H
#include "cplayer.h"
#include <QTcpSocket>

class CClient :public CPlayer
{
public:
    CClient();
    void ShowFinalResult();
private:
    QTcpSocket *client_socket_;
};

#endif // CCLIENT_H
