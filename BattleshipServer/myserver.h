#ifndef CSERVER_H
#define CSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QtNetwork>
#include "cfield.h"
#include "cuser.h"


class CServer
{
public:
    CServer();
public slots:
    void newConnection();
private:
    int port_number_;
    bool is_finished_;
    QHostAddress address_;
    QTcpServer *tcp_server_;
    CUser *clients_;
    CField *fields_;
};

#endif // CSERVER_H
