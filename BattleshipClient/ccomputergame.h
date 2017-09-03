#ifndef CCOMPUTERGAME_H
#define CCOMPUTERGAME_H

#include <QMessageBox>
#include "cgamemodel.h"
#include "ccomputeropponent.h"


class CComputerGame
{
public:
    CComputerGame(CGameModel *model,CGameModel *computermodel,CComputerOpponent *computeropponent);
    bool startGame();
    void enemyMove();
    bool isKilled(int x, int y, CGameModel *model);
    bool isCellsEmptyNextTo(int x, int y);
    bool isShipNextTo(int x, int y);
    bool isInjured(int x, int y);
    bool isAllCellsInjured(int x, int y);
    void setKilled(int x, int y, int vertical, int right);
    void endGame(const std::string &message);
    static constexpr int mNumberOfAllShips = 20;
private:
    CGameModel *mModel;
    CGameModel *mComputerModel;
    CGameModel *mCurrentModel;
    CComputerOpponent *mComputerOpponent;

    QMessageBox mMessageBox;
};

#endif // CCOMPUTERGAME_H
