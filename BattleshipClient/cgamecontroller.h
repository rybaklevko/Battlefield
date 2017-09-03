#ifndef CGAMECONTROLLER_H
#define CGAMECONTROLLER_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <iostream>
#include <string>
#include "cgamemodel.h"
#include "ccomputeropponent.h"
#include "ccomputergame.h"

class CGameController : public QWidget
{
    Q_OBJECT
public:
    CGameController(CGameModel *model);
    void setGameMode(EGameMode mode);
    EGameMode getGameMode();
    void parseData(QString data);
    void enemyMove();
    void enemyResult(ECell state);
    void enemyPlacement();
    std::string serverMessage();
    bool makeConnection();
    void sendData(const QString &data);
    void sendPlacement();
    void mouseLeftButtonClicked(QPoint &position );
    void mouseRightButtonClicked(QPoint &position);
    void randomFields();

    bool startGame();
    void makeEnemyMove();

signals:
    void signalStateChanged();

public slots:
    void slotReadDataFromSocket();
private:
    QTcpSocket *mSocket;
    std::string mReceivedData;
    CGameModel *mModel;
    CGameModel *mComputerModel;
    CComputerOpponent *mComputerOpponent;
    CComputerGame *mComputerGame;
    QMessageBox mMessageBox;
    EGameMode mGameMode;
};

#endif // CGAMECONTROLLER_H
