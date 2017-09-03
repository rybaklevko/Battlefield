#include "myserver.h"

CServer::CServer()
{
    tcp_server_ = new QTcpServer();

    connect(tcp_server_,SIGNAL(newConnection()),this,SLOT(newConnection()));

    if(!tcp_server_->listen((QHostAddress::Any),1234))
    {

        qDebug()<<"Error with connection!";
    }
    else
    {
        qDebug()<<"Started!";

    }
}

void CServer::newConnection()
{
    QTcpSocket *socket = tcp_server_->nextPendingConnection();
    socket->waitForBytesWritten(1000);
    socket->waitForReadyRead(3000);
    qDebug()<<socket->bytesAvailable();
    QByteArray data = socket->readAll();
    QString str = data;
    qDebug()<<"STR:"<<str;
    qDebug()<<"Data"<<data;

    socket->write("Connected!Wait for enemy....");
    socket->flush();
}
