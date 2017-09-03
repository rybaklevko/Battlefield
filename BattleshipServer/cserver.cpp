#include "cserver.h"

//constructor, put server into listening position
//call connection socket,after client was connected
//and try to make connection between users each 3 seconds
CServer::CServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer();

    //if client connected
    connect(mTcpServer,SIGNAL(newConnection()),this,SLOT(slotNewClientConnection()));

    if(!mTcpServer->listen(kDefaultAddress,kDefaultPortNumber)) {
        qDebug()<<"Error with connection!";
    } else {
        qDebug()<<"Started!";
    }

    //try to connect players each 3 seconds
    mTimeCounter = new QTimer(this);
    mTimeCounter->setInterval(kDefaultTimeValue);
    connect(mTimeCounter,SIGNAL(timeout()),this,SLOT(slotTimerConnection()));
    mTimeCounter->start();
}


//function,which parse received data
void CServer::parseData(QString data, int userID)
{
    currentUser = mClients.find(userID);
    mReceivedData= data.toStdString();
    switch (mReceivedData.at(0)) {
    case 'p':
        cellPlacement();
        break;
    case 's':
        makeMove();
        break;
    default:
        clientMessage(data);
        break;
    }
}

//initialize placement for current user
bool CServer::cellPlacement()
{
    currentUser->mField.cellInitializatiion(mReceivedQStringData);
    return true;
}

//set move information on server and send it to both of players
//also,check does the game was finished
void CServer::makeMove()
{
    int x = mReceivedData.at(1) - '0';
    int y = mReceivedData.at(2) - '0';
    //QMap<int,CUser>::iterator currentUser = mClients.find(userID);

    //check does ship on this cell was faulty/killed
    if (currentUser->mEnemyPlayer->mField.getCell(x,y) == kInjured) {
        qDebug()<<"Cell was already marked!";
        currentUser->sendData("Go");
        return;
    }

    if (currentUser->mEnemyPlayer->mField.getCell(x,y) == kKilled) {
        qDebug()<<"Ship was already killed";
        currentUser->sendData("Go");
        return;
    }

    //if shoot is well-aimed
    if (currentUser->mEnemyPlayer->mField.getCell(x,y) == kShip) {

        //mark it as injured first
        currentUser->mField.setCell(x,y,kInjured);
        currentUser->mEnemyPlayer->mField.setCell(x,y,kInjured);

        //if ship was killed mark it as killed
        //if no still it just injured
        if( !isKilled(x,y)) {
             QString str = "i";

            str.push_back(QString::number(x));
            str.push_back(QString::number(y));

            currentUser->sendData(str);

            str.push_front("s");

            currentUser->mEnemyPlayer->sendData(str);
            currentUser->mState = kMakingStep;
            currentUser->mEnemyPlayer->mState = kWaitingForEnemyStep;
            //std::this_thread::sleep_for(std::chrono::seconds(0.3));
            //currentUser->sendData("Go");
        }

        currentUser->mNumberOfMarkedCells++;
        //if all cells were marked
        if(currentUser->mNumberOfMarkedCells >= mCellsNumber) {
            //QThread::msleep(1000);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            currentUser->mTcpSocket->flush();
            currentUser->mEnemyPlayer->mTcpSocket->flush();
            currentUser->sendData("Win");
            currentUser->mEnemyPlayer->sendData("Lose");
        }

    } else { //if missed mark it as dot
        currentUser->mField.setCell(x,y,kDot);
        currentUser->mEnemyPlayer->mField.setCell(x,y,kDot);

        QString str="m";
        str.push_back(QString::number(x));
        str.push_back(QString::number(y));

        currentUser->sendData(str);

        str.push_front("s");
        currentUser->mEnemyPlayer->sendData(str);
        currentUser->mState = kWaitingForEnemyStep;
        currentUser->mEnemyPlayer->mState = kMakingStep;
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        //currentUser->mEnemyPlayer->sendData("Go");
    }
}

//check does ship was killed
bool CServer::isKilled(int x,int y)
{
    //if this is one-size ship set it as killed
    if((isCellsEmptyNextTo( x + 1, y))
        && (isCellsEmptyNextTo( x - 1, y))
        && (isCellsEmptyNextTo( x, y + 1))
        && (isCellsEmptyNextTo( x, y - 1))) {
        setKilled(x,y,0,1);
        qDebug()<<"1 size ship";
        return true;
    }

    if(isShipNextTo(x,y)) {
        qDebug()<<"Not killed yet";
        return false;
    }


    return isAllCellsInjured(x,y);
}

//check does cell isn't marked with ship,killed,injured
bool CServer::isCellsEmptyNextTo(int x, int y)
{
    if((currentUser->mEnemyPlayer->mField.getCell(x,y) == kEmpty)
       || (currentUser->mEnemyPlayer->mField.getCell(x,y) == kDot)
       || (currentUser->mEnemyPlayer->mField.getCell(x,y) == kOutofField)) {
        return true;
    }
    return false;
}

//check does there is ship next to cell
bool CServer::isShipNextTo(int x, int y)
{
    if ((currentUser->mEnemyPlayer->mField.getCell(x + 1, y) == kShip)
        || (currentUser->mEnemyPlayer->mField.getCell(x - 1, y) == kShip)
        || (currentUser->mEnemyPlayer->mField.getCell(x, y + 1) == kShip)
        || (currentUser->mEnemyPlayer->mField.getCell(x, y - 1) == kShip)) {
        return true;
    }
    return false;
}

//check does cell is injured
bool CServer::isInjured(int x,int y)
{
    if( currentUser->mEnemyPlayer->mField.getCell( x, y) == kInjured) {
        return true;
    }
    return false;
}

//check does all ship's cells are marked
bool CServer::isAllCellsInjured(int x, int y)
{
    int i = 0;
    if( isInjured(x + 1, y)) {
        if( !isInjured( x - 1, y)) {
            while(isInjured(x + i, y)) {
                i++;
            }
            if( currentUser ->mEnemyPlayer->mField.getCell( x + i, y) == kShip) {
                return false;
            } else {
                setKilled(x,y,0,1);
                return true;
            }
        } else {
            return false;
        }
    } else if( isInjured(x - 1, y)) {
        if( !isInjured( x + 1, y)) {
            while(isInjured(x - i, y)) {
                i++;
            }
            if( currentUser ->mEnemyPlayer->mField.getCell( x - i, y) == kShip) {
                return false;
            } else {
                setKilled(x,y,0,-1);
                return true;
            }
        } else {
            return false;
        }
    } else if( isInjured( x, y + 1)) {
        if( !isInjured( x, y - 1)) {
            while(isInjured( x, y + i)) {
                i++;
            }
            if( currentUser ->mEnemyPlayer->mField.getCell( x, y + i ) == kShip) {
                return false;
            } else {
                setKilled(x,y,1,0);
                return true;
            }
        } else {
            return false;
        }
    } else if( isInjured( x, y - 1)) {
        if( !isInjured( x, y + 1)) {
            while(isInjured( x, y - i)) {
                i++;
            }
            if( currentUser ->mEnemyPlayer->mField.getCell( x, y - i ) == kShip) {
                return false;
            } else {
                setKilled(x,y,-1,0);
                return true;
            }
        } else {
            return false;
        }
    }
}

//set whole ship as killed
//send information about it to both of users
//horizontal and vertical  parametres determines ship placement
void CServer::setKilled(int x, int y, int vertical, int horizontal)
{
    if(horizontal) {
        while(isInjured(x,y)) {
            QString str = "k";
            currentUser->mField.setCell(x,y,kKilled);
            currentUser->mEnemyPlayer->mField.setCell(x,y,kKilled);
            str.push_back(QString::number(x));
            str.push_back(QString::number(y));
            currentUser->sendData(str);

            str.push_front("s");

            currentUser->mEnemyPlayer->sendData(str);

            x+=horizontal;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            currentUser->mTcpSocket->flush();
            currentUser->mEnemyPlayer->mTcpSocket->flush();
        }
    } else if(vertical) {
        while(isInjured(x,y)) {
            QString str = "k";
            currentUser->mField.setCell(x,y,kKilled);
            currentUser->mEnemyPlayer->mField.setCell(x,y,kKilled);
            str.push_back(QString::number(x));
            str.push_back(QString::number(y));
            currentUser->sendData(str);

            str.push_front("s");

            currentUser->mEnemyPlayer->sendData(str);

            y += vertical;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            currentUser->mTcpSocket->flush();
            currentUser->mEnemyPlayer->mTcpSocket->flush();
        }
    }

    currentUser->mState = kMakingStep;
    currentUser->mEnemyPlayer->mState = kWaitingForEnemyStep;
    currentUser->sendData("Go");
}

//function,which connects two users
//get iterators of both of them and connect
void CServer::makeConnection(QMap<int,CUser>::iterator currentClient, QMap<int,CUser>::iterator opponentUser)
{
    currentClient->mState = kMakingStep;
    opponentUser->mState = kWaitingForEnemyStep;

    currentClient->mEnemyPlayer = opponentUser;
    opponentUser->mEnemyPlayer = currentClient;


    //QThread::msleep(1000);
    currentClient->mTcpSocket->write("Go");
    opponentUser->mTcpSocket->write("Enemy was found!");

    currentClient->mTcpSocket->flush();

}

void CServer::clientMessage(QString data)
{
    qDebug()<<"Message from client: "<<data;
    if(data.startsWith("Go")) {
        currentUser->mEnemyPlayer->sendData(data);
    }
}

//socket,called,when new user is connected
void CServer::slotNewClientConnection()
{
    CUser user;
    user.mTcpSocket = mTcpServer->nextPendingConnection();
    user.mEnemyPlayer = mClients.end();
    int userId = user.mTcpSocket->socketDescriptor();
    user.mState = kWaitingForEnemyConnection;
    qDebug()<<user.mTcpSocket->socketDescriptor();
    mClients.insert(userId,user);

    connect(user.mTcpSocket,SIGNAL(readyRead()),this,SLOT(slotReadDataFromSocket()));
    connect(user.mTcpSocket,SIGNAL(disconnected()),this,SLOT(slotClientDisconnect()));
    //user.sendData("Connected!Wait for enemy..");
}

//socket,when disconnected
void CServer::slotClientDisconnect()
{
    qDebug()<<"Disconnected..";
}

//socket,which read data from socket
void CServer::slotReadDataFromSocket()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    socket->waitForReadyRead(kDefaultTimeValue);
    QByteArray data = socket->readAll();
    QString receivedData = data;
    mReceivedQStringData = receivedData;
    qDebug()<<"Message: "<<data;
    parseData(receivedData,socket->socketDescriptor());
}

//function,which found enemy
//for current user, information about it holds as iterator
void CServer::slotTimerConnection()
{
    auto opponentClient = mClients.end();
    for(auto it = mClients.begin();it!=mClients.end();++it) {
        if((*it).mState == kWaitingForEnemyConnection) {
            if(opponentClient == mClients.end()) {
                opponentClient = it;
            } else {
                makeConnection(opponentClient,it);
                opponentClient = mClients.end();
            }

        }

    }
}
