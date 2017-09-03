#include "cfield.h"
#include <iostream>
//constructor
//
CField::CField()
{
    mAllCells.resize(kSize);
    mAllCellsBooleanForm.resize(kSize);

    //create space for 100 elements
    for(int i(0); i < kSize; i++) {
        mAllCells[i].resize(kSize);
        mAllCellsBooleanForm[i].resize(kSize);
    }
    clearField();
}

void CField::clearField()
{
    //set all field as empty
    for(int i(0); i < kSize; i++) {
        std::fill(mAllCells[i].begin(),mAllCells[i].end(),kEmpty);
        std::fill(mAllCellsBooleanForm[i].begin(),mAllCellsBooleanForm[i].end(),0);
    }
}


//set cell value(type of cell)
void CField::setCell(int x, int y, ECell cell)
{
    if ( (x < 0)  || (y < 0)
            || (x >= kSize) || (y >= kSize)) {
        qDebug() <<"Wrong position!Cell wasn't set";
    } else {
        mAllCells[x][y] = cell;
        if(cell == kShip) {
            mAllCellsBooleanForm[x][y] = 1;
        }
    }
}

//set placement in boolean form
void CField::setPlacement(const QVector<QVector<bool>> &allcell)
{
    mAllCellsBooleanForm = allcell;
}

//return cell value on choosed position
ECell CField::getCell(int x, int y)
{
    if ( (x < 0)  || (y < 0)
            || (x >= kSize) || (y >= kSize)) {
        return kOutofField;
    } else {
        return mAllCells[x][y];
    }
}

//function,which change coordinates form
//from coordinates form to squares form
QPoint CField::changeCoordinatesType(int x, int y,ECurrentField field)
{
    //use wrong position values as default
    QPoint currentposition(-1,-1);
    int left;
    if(field == kEnemyField) {
        left = kFieldOpponentLeft;
    } else {
        left = kFieldUserLeft;
    }

    //check is position correct
    if (!((x < left) || (x > (left + kFieldWidth))
         || (y < kFieldTop) || (y > (kFieldTop + kFieldHeight)))) {

        int pointX = 1.0 * (x - left)
                /(1.0 * kFieldWidth / double(kSize));
        int pointY = 1.0 * (y - kFieldTop)
                /(1.0 * kFieldHeight / double(kSize));

        if (isCorrectShipPlace(pointX,pointY)) {
            currentposition.setX(pointX);
            currentposition.setY(pointY);
        }
    }
    return currentposition;
}

//function,which check correct try to place ships
bool CField::isCorrectShipPlace(int x, int y)
{
    //Check does another ship is placed next to current position
    if ((getCell( x - 1,  (y - 1)) == kShip)
            || (getCell( x + 1,  (y - 1)) == kShip)
            || (getCell( x - 1,  (y + 1)) == kShip)
            || (getCell( x + 1 , (y + 1)) == kShip)) {
        return false;
    }
    return true;
}


//get placement of ships in boolean form
QVector<QVector<bool> > CField::getPlacement()
{
    return mAllCellsBooleanForm;
}

