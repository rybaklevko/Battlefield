#ifndef CSERVER_H
#define CSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QByteArray>
#include <QString>
#include <QtNetwork>
#include <QMap>
#include <QTimer>
#include <thread>
#include <chrono>
#include "cuser.h"


class CServer : public QObject
{
    Q_OBJECT
public:
    explicit CServer(QObject *parent = 0);
    void parseData(QString data, int userID);
    bool cellPlacement();
    void makeMove();
    bool isKilled(int x, int y);
    bool isCellsEmptyNextTo(int x, int y);
    bool isShipNextTo(int x, int y);
    bool isInjured(int x, int y);
    bool isAllCellsInjured(int x, int y);
    void setKilled(int x, int y, int vertical, int right);
    void makeConnection(QMap<int,CUser>::iterator currentClient, QMap<int,CUser>::iterator opponentUser);
    void clientMessage(QString data);

signals:

public slots:
    void slotNewClientConnection();
    void slotClientDisconnect();
    void slotReadDataFromSocket();
    void slotTimerConnection();
private:
    bool mIsFinished;
    static constexpr int mCellsNumber = 20;
    QTcpServer *mTcpServer;
    std::string mReceivedData;
    QString mReceivedQStringData;
    QMap<int,CUser>mClients;
    QMap<int,CUser>::iterator currentUser;
    QTimer *mTimeCounter;
};

#endif // CSERVER_H
