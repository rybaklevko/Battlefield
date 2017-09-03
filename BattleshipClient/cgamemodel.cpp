#include "cgamemodel.h"

//constructor
CGameModel::CGameModel(QObject *parent) : QObject(parent)
{
    mPlayerField = new CField(); // placement of player field
    mEnemyField = new CField(); //placement of enemy field
}

CGameModel::~CGameModel()
{
    delete mPlayerField;
    delete mEnemyField;
}

//set fields as empty depends on field
void CGameModel::clearField(ECurrentField field)
{
    if(field == kEnemyField) {
        mEnemyField->clearField();
    } else {
        mPlayerField->clearField();
    }
}

//get cell depends on field
ECell CGameModel::getCell(int x, int y, ECurrentField field)
{
    if (field == kEnemyField) {
       return mEnemyField->getCell(x,y);
    } else {
        return mPlayerField->getCell(x,y);
    }
}

//set cell depends on field
void CGameModel::setCell(int x, int y, ECell cell, ECurrentField field)
{
    if(field == kEnemyField) {
        mEnemyField->setCell(x,y,cell);
    } else {
        mPlayerField->setCell(x,y,cell);
    }
}

//change coordinates type depends on field
QPoint CGameModel::changeCoordinatesType(int x, int y, ECurrentField field)
{
    if(field == kEnemyField) {
        return mEnemyField->changeCoordinatesType(x,y,field);
    } else {
        return mPlayerField->changeCoordinatesType(x,y,field);
    }
}

//get placement depends on field
QVector<QVector<bool> > CGameModel::getPlacement(ECurrentField field)
{
    if(field == kEnemyField) {
        return mEnemyField->getPlacement();
    } else {
        return mPlayerField->getPlacement();
    }
}

//set current state
void CGameModel::setState(ECurrentState state)
{
    mState = state;
}

//get current state
ECurrentState CGameModel::getState()
{
    return mState;
}

//check each size and number of ship with that size on correctness
bool CGameModel::isCorrectShipPlacement()
{
    if (!(shipNumbers(4) == 1)) {
        qDebug()<<"Wrong number of 4-size ships";
        return false;
    } else if(!(shipNumbers(3) == 2)) {
        qDebug()<<"Wrong number of 3-size ships";
        return false;
    } else if(!(shipNumbers(2) == 3)) {
        qDebug()<<"Wrong number of 2-size ships";
        return false;
    } else if (!(shipNumbers(1) == 4)) {
        qDebug()<<"Wrong number of 1-size ships";
        return false;
    }
    return true;
}

bool CGameModel::isCorrectShipPlace(int x, int y,ECurrentField field)
{
    if (field == kEnemyField) {
       return mEnemyField->isCorrectShipPlace(x,y);
    } else {
        return mPlayerField->isCorrectShipPlace(x,y);
    }
}

//count number of ships with some size
int CGameModel::shipNumbers(int size)
{
    int shipNumber = 0;
    for(int i(0); i < kSize; i++) {
        for(int j(0); j < kSize; j++){
            if (isShip(size,i,j)) {
                shipNumber++;
            }
        }
    }
    return shipNumber;
}

//check does current ship has correct size,placement etc
bool CGameModel::isShip(int size, int x, int y)
{
    //check just cells with ship
    if (mPlayerField->getCell( x, y) != kShip) {
        return false;
    }

    //check begin just from first cell with ship
    if (mPlayerField->getCell( x - 1, y) == kShip) {
        return false;
    }
    if (mPlayerField->getCell( x, y - 1) == kShip) {
        return false;
    }

    //if this is smallest ship
    if(size == 1) {
        if(mPlayerField->getCell( x + 1, y) == kShip
           || mPlayerField->getCell(x,y + 1) == kShip) {
            return false;
        } else {
            return true;
        }

    } else {
        //if this isn't 1-cell size ship
        //check is this ship by horizontal
        if (mPlayerField->getCell( x + 1, y) == kShip) {
           int i=0;
           while (mPlayerField->getCell( x + i, y) == kShip) {
               i++;
           }
           if(i != size) {
               return false;
           } else {
               return true;
           }
        }
        //check is this ship by vertical
        if (mPlayerField->getCell( x, y + 1) == kShip) {
           int i = 0;
           while (mPlayerField->getCell( x, y + i) == kShip) {
               i++;
           }
           if(i != size) {
               return false;
           } else {
               return true;
           }
        }
    }
    return false;
}

//generate random ship placement
void CGameModel::randomShip(int size)
{
    bool isPlaced = false;
    int vertical = qrand() % 2;
    int x,y;
    int i;

    while (!isPlaced) {

        //generate random numbers
        QTime time = QTime::currentTime();
        qsrand(uint(time.msec()));
        x = qrand() % 10;
        y = qrand() % 10;

        i = 0;

        isPlaced = true;

        if(getCell( x, y, kPlayerField) == kShip) {
            isPlaced = false;
            continue;
        }

        //check cells for correct placement
        //if ship has 1 cell size
        if( size == 1) {
            if((getCell( x + 1, y, kPlayerField) == kShip)
               || (getCell( x - 1, y, kPlayerField) == kShip)
               || (getCell( x, y + 1, kPlayerField) == kShip)
               || (getCell( x, y - 1, kPlayerField) == kShip)
               || (!isCorrectShipPlace( x, y, kPlayerField))) {
                isPlaced = false;
                continue;
            }
        } else {//if ship is bigger than 1
            if(vertical == 1) {
                if((getCell( x, y - 1, kPlayerField) == kShip)
                   || (getCell( x, y + size, kPlayerField) == kShip)) {
                    isPlaced = false;
                    continue;
                }

                while( i < size ) {
                    if((!isCorrectShipPlace( x, y + i, kPlayerField))
                       || ((y + i) > 9)
                       || (getCell( x, y + i, kPlayerField) == kShip)) {
                        isPlaced = false;
                        break;
                    }
                    i++;
                }
            } else {
                if((getCell(x - 1, y, kPlayerField) == kShip)
                        || (getCell( x + size, y, kPlayerField) == kShip)) {
                    isPlaced = false;
                    continue;
                }
                while( i < size) {
                    if((!isCorrectShipPlace( x + i, y, kPlayerField))
                       || ((x + i) > 9)
                       || (getCell( x + i, y, kPlayerField) == kShip)) {
                        isPlaced = false;
                        break;
                    }
                    i++;
                }
            }
        }
    }

    i = 0 ;

    //after cells were checked for corectness
    //set them
    if ( vertical == 1 ) {
        while(i < size) {
            setCell( x, y + i, kShip, kPlayerField);
            i++;
        }
    } else {
        while( i < size ) {
            setCell( x + i, y, kShip, kPlayerField);
            i++;
        }
    }
}

void CGameModel::addInjured()
{
    mNumberOfInjured++;
}

int CGameModel::getNumberOfInjured()
{
    return mNumberOfInjured;
}
