#include "cgamecontroller.h"

//constructor,initialize socket and contains connection for receiving data slot
CGameController::CGameController(CGameModel *model):mModel(model)
{
    mSocket = new QTcpSocket();

    mComputerModel = new CGameModel();
    mComputerOpponent = new CComputerOpponent(mComputerModel);

    mComputerGame = new CComputerGame(mModel,mComputerModel,mComputerOpponent);

    connect(mSocket,SIGNAL(readyRead()),this,SLOT(slotReadDataFromSocket()));
}

void CGameController::setGameMode(EGameMode mode)
{
    mGameMode = mode;
}

EGameMode CGameController::getGameMode()
{
    return mGameMode;
}

//function,which parse received data
//data receives in from like "letter" "x" "y"
//"letter" depends on current state
//p - placement, s - step(move),m - miss,i - injured, k - killed
//x - horizontal position, y - vertical position
void CGameController::parseData(QString data)
{
    mReceivedData= data.toStdString();
    switch (mReceivedData.at(0)) {
    case 'p':
        enemyPlacement();
        break;
    case 's':
        enemyMove();
        break;
    case 'm':
        enemyResult(kDot);
        break;
    case 'i':
        enemyResult(kInjured);
        break;
    case 'k':
        enemyResult(kKilled);
        break;
    default:
        serverMessage();
        break;
    }
}

//if enemy move was received
//set on player field result
void CGameController::enemyMove()
{
    int x = mReceivedData.at(2) -'0';
    int y = mReceivedData.at(3) - '0';
    if (mReceivedData.at(1) == 'm') {
        mModel->setCell(x,y,kDot,kPlayerField);
        mModel->setState(kMakingStep);
    } else if(mReceivedData.at(1) == 'i') {
        mModel->setCell(x,y,kInjured,kPlayerField);
        sendData("Go");
        mModel->setState(kWaitingForMove);
    } else if (mReceivedData.at(1) == 'k') {
        mModel->setCell(x,y,kKilled,kPlayerField);
    }
}

//result of our move
//displays it
void CGameController::enemyResult(ECell state)
{
    int x = mReceivedData.at(1) - '0';
    int y = mReceivedData.at(2) - '0';
    mModel->setCell(x,y,state,kEnemyField);
}

//receive enemy placement in boolean form
void CGameController::enemyPlacement()
{
    QVector<bool>allcell;
    for (auto it = mReceivedData.begin()+1; it != mReceivedData.end(); ++it) {
        allcell.push_back(*it - '0');
    }
}

//if we don't receive coded message
//show message box,if game was finished
std::string CGameController::serverMessage()
{
    if (mReceivedData == "Go") {
        mModel->setState(kMakingStep);
    } else if (mReceivedData.find("Win") != std::string::npos) {
        mModel->setState(kGameEnd);
        mMessageBox.setText("You win!");
        mMessageBox.exec();
    } else if (mReceivedData.find("Lose") != std::string::npos) {
        mModel->setState(kGameEnd);
        mMessageBox.setText("You lose!");
        mMessageBox.exec();
    }
    return mReceivedData;
}

//function,which try to make connection with server
bool CGameController::makeConnection()
{
    if (!(mModel->isCorrectShipPlacement())) {
        qDebug()<<"Error! Not correct placement";
        return false;
    }
    mSocket->connectToHost(kDefaultAddress,kDefaultPortNumber);
    if(mSocket->waitForConnected(kDefaultTimeValue)) {
        qDebug()<<"Connected!";
    } else {
        qDebug()<<"Wasn't connected!";
        mSocket->close();
        return false;
    }
    return true;
}

//function,which send data
void CGameController::sendData(const QString &data)
{
    mSocket->write(data.toUtf8());//,data.length());
    mSocket->flush();
    mSocket->waitForBytesWritten(kDefaultTimeValue);
}

//function,which send placement
void CGameController::sendPlacement()
{
    QVector<QVector<bool>>placement(mModel->getPlacement(kPlayerField));
    QString stringplacement = "p";
    for(int i(0); i < kSize; i++) {
        for(int j(0); j < kSize; j++) {
            stringplacement.push_back(QString::number(placement[i][j]));
        }
    }
    sendData(stringplacement);
}

//make action after mouse press
void CGameController::mouseLeftButtonClicked(QPoint &position)
{
    //depends on current state make action
    switch (mModel->getState()) {
    case (kPlacingShips): {
        QPoint point = mModel->changeCoordinatesType(position.x(),position.y(),kPlayerField);

        if(point.x() == - 1) return; //break process,if wrong position was choosen

        mModel->setCell(point.x(),point.y(),kShip,kPlayerField);
        emit signalStateChanged();//emit signal,which activate slot to make this->update
        break;
    }
    case (kMakingStep): {
        QPoint point = mModel->changeCoordinatesType(position.x(),position.y(),kEnemyField);

        if(point.x() == - 1) return; //break process,if wrong position was choosen

        //if playe with real player
        if(getGameMode() == kRealOpponent) {
            //if user makes move, send string with information about state and position(by x and by y)
            QString str="s";
            str.push_back(QString::number(point.x()));
            str.push_back(QString::number(point.y()));
            sendData(str);
            mModel->setState(kWaitingForMove);
        } else {//if play with computer opponent
            if (mComputerModel->getCell(point.x(),point.y(),kPlayerField) == kShip) {
                mModel->setCell(point.x(),point.y(),kInjured,kEnemyField);
                mComputerModel->setCell(point.x(),point.y(),kInjured,kPlayerField);

                mComputerModel->addInjured();

                mComputerGame->isKilled(point.x(),point.y(),mComputerModel);

                if(mComputerModel->getNumberOfInjured() == mComputerGame->mNumberOfAllShips) {
                    mComputerGame->endGame("Win");
                }

            } else {
                mModel->setCell(point.x(),point.y(),kDot,kEnemyField);
                mComputerModel->setCell(point.x(),point.y(),kDot,kPlayerField);
                mModel->setState(kWaitingForMove);
                mComputerModel->setState(kMakingStep);
                makeEnemyMove();
            }
        }

        emit signalStateChanged();
        break;
    }

    }
}

//click right button to remove ships
void CGameController::mouseRightButtonClicked(QPoint &position)
{
    if (mModel->getState() == kPlacingShips) {
        QPoint point = mModel->changeCoordinatesType(position.x(),position.y(),kPlayerField);

        if (point.x() == - 1) return;

        if (mModel->getCell(point.x(),point.y(),kPlayerField) == kShip) {
            mModel->setCell(point.x(),point.y(),kEmpty,kPlayerField);
        }
    }
}

//call function for random placing of each ship
void CGameController::randomFields()
{
    mModel->clearField(kPlayerField);

    mModel->randomShip(4);
    mModel->randomShip(3);
    mModel->randomShip(3);
    mModel->randomShip(2);
    mModel->randomShip(2);
    mModel->randomShip(2);
    mModel->randomShip(1);
    mModel->randomShip(1);
    mModel->randomShip(1);
    mModel->randomShip(1);
    emit signalStateChanged();
}

//if game with computer is started
bool CGameController::startGame()
{
    if( !mComputerGame->startGame()) {
        return false;
    }
    return true;
}

//generate computer move,update information
void CGameController::makeEnemyMove()
{
    mComputerGame->enemyMove();
    emit signalStateChanged();
}

//slot,which read received data
void CGameController::slotReadDataFromSocket()
{
    mSocket->waitForReadyRead(kDefaultTimeValue);

    QByteArray data = mSocket->readAll();
    QString receivedData = data;
    qDebug()<<"Message: "<<data;
    parseData(receivedData);
}
