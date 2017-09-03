#include "ccomputeropponent.h"

CComputerOpponent::CComputerOpponent(CGameModel *computermodel)
    :mComputerModel(computermodel)
{

}

//generate placement of ships for computer
void CComputerOpponent::generatePlacement()
{
    mComputerModel->randomShip(4);
    mComputerModel->randomShip(3);
    mComputerModel->randomShip(3);
    mComputerModel->randomShip(2);
    mComputerModel->randomShip(2);
    mComputerModel->randomShip(2);
    mComputerModel->randomShip(1);
    mComputerModel->randomShip(1);
    mComputerModel->randomShip(1);
    mComputerModel->randomShip(1);
}

//generate move for computer
QPoint CComputerOpponent::generateMove()
{
    QPoint currentpostion;
    QTime time = QTime::currentTime();
    qsrand(uint(time.msec()));
    int x = qrand() % 10;
    int y = qrand() % 10;
    currentpostion.setX(x);
    currentpostion.setY(y);
    return currentpostion;
}
