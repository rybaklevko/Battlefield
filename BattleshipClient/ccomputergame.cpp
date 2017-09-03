#include "ccomputergame.h"

CComputerGame::CComputerGame(CGameModel *model, CGameModel *computermodel, CComputerOpponent *computeropponent):
    mModel(model),mComputerModel(computermodel),mComputerOpponent(computeropponent)
{

}

bool CComputerGame::startGame()
{
    mComputerOpponent->generatePlacement();
    if (!(mModel->isCorrectShipPlacement())) {
        qDebug()<<"Error! Not correct placement";
        return false;
    }

    if (!(mComputerModel->isCorrectShipPlacement())) {
        qDebug()<<"Error! Not correct placement";
        return false;
    }

    mModel->setState(kMakingStep);
    mComputerModel->setState(kWaitingForMove);

    return true;
}

void CComputerGame::enemyMove()
{
    while (mComputerModel->getState() == kMakingStep) {
        QPoint point  = mComputerOpponent->generateMove();
        while ( (mModel->getCell(point.x(), point.y(),kPlayerField) == kInjured)
                || (mModel->getCell(point.x(), point.y(),kPlayerField) == kDot)) {
            point = mComputerOpponent->generateMove();
        }

        if (mModel->getCell(point.x(),point.y(),kPlayerField) == kShip) {
            mModel->setCell(point.x(),point.y(),kInjured,kPlayerField);
            mComputerModel->setCell(point.x(),point.y(),kInjured,kEnemyField);

            mModel->addInjured();

            isKilled(point.x(),point.y(),mModel);

            if(mModel->getNumberOfInjured() == mNumberOfAllShips) {
                endGame("Lose");
            }

        } else {
            mModel->setCell(point.x(),point.y(),kDot,kPlayerField);
            mComputerModel->setCell(point.x(),point.y(),kDot,kEnemyField);
            mModel->setState(kMakingStep);
            mComputerModel->setState(kWaitingForMove);
        }
    }
}


//check does ship was killed
bool CComputerGame::isKilled(int x, int y, CGameModel *model)
{
    mCurrentModel = model;
    //if this is one-size ship set it as killed
    if((isCellsEmptyNextTo( x + 1, y))
        && (isCellsEmptyNextTo( x - 1, y))
        && (isCellsEmptyNextTo( x, y + 1))
        && (isCellsEmptyNextTo( x, y - 1))) {
        setKilled(x,y,0,1);
        return true;
    }

    if(isShipNextTo(x,y)) {
        return false;
    }

    return isAllCellsInjured(x,y);
}

//check does cell isn't marked with ship,killed,injured
bool CComputerGame::isCellsEmptyNextTo(int x, int y)
{
    if((mCurrentModel->getCell(x,y,kPlayerField) == kEmpty)
       || (mCurrentModel->getCell(x,y,kPlayerField) == kDot)
       || (mCurrentModel->getCell(x,y,kPlayerField) == kOutofField)) {
        return true;
    }
    return false;
}

//check does there is ship next to cell
bool CComputerGame::isShipNextTo(int x, int y)
{
    if ((mCurrentModel->getCell(x + 1, y, kPlayerField) == kShip)
        || (mCurrentModel->getCell(x - 1, y, kPlayerField) == kShip)
        || (mCurrentModel->getCell(x, y + 1, kPlayerField) == kShip)
        || (mCurrentModel->getCell(x, y - 1, kPlayerField) == kShip)) {
        return true;
    }
    return false;
}

//check does cell is injured
bool CComputerGame::isInjured(int x, int y)
{
    if( mCurrentModel->getCell( x, y, kPlayerField) == kInjured) {
        return true;
    }
    return false;
}

//check does all ship's cells are marked
bool CComputerGame::isAllCellsInjured(int x, int y)
{
    int i = 0;
    if( isInjured(x + 1, y)) {
        if( !isInjured( x - 1, y)) {
            while(isInjured(x + i, y)) {
                i++;
            }
            if( mCurrentModel->getCell( x + i, y, kPlayerField) == kShip) {
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
            if( mCurrentModel->getCell( x - i, y, kPlayerField) == kShip) {
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
            if( mCurrentModel->getCell( x, y + i, kPlayerField ) == kShip) {
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
            if( mCurrentModel->getCell( x, y - i, kPlayerField ) == kShip) {
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
void CComputerGame::setKilled(int x, int y, int vertical, int horizontal)
{
    CGameModel *modelOpponent;
    if(mCurrentModel == mModel) {
        modelOpponent = mComputerModel;
    } else {
        modelOpponent = mModel;
    }

    if(horizontal) {
        while(isInjured(x,y)) {
            mCurrentModel->setCell(x,y,kKilled,kPlayerField);
            modelOpponent->setCell(x,y,kKilled,kEnemyField);

            x+=horizontal;
        }
    } else if(vertical) {
        while(isInjured(x,y)) {
            mCurrentModel->setCell(x,y,kKilled,kPlayerField);
            modelOpponent->setCell(x,y,kKilled,kEnemyField);

            y += vertical;
        }
    }
}

//if game was over
void CComputerGame::endGame(const std::string &message)
{
        if(message =="Win") {
            mMessageBox.setText("You win!");
            mMessageBox.exec();
        } else {
            mMessageBox.setText("You lose!");
            mMessageBox.exec();
        }
        mModel->setState(kGameEnd);
}
