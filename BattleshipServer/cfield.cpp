#include "cfield.h"

//set cell value(type of cell)
void CField::cellInitializatiion(QString &data)
{
    auto it = data.begin() + 1;
    for(int i(0); i < kSize; i++) {
        mAllCells.push_back(QVector<ECell>());
        mAllCellsBooleanForm.push_back(QVector<bool>());
        for(int j(0);j < kSize; j++) {
            mAllCells[i].push_back(ECell(QString(*it).toInt()));
            mAllCellsBooleanForm[i].push_back(QString(*it).toInt());
            it++;
        }
    }
}

//set cell value(type of cell)
void CField::setCell(int x, int y, ECell cell)
{
    if ( (x < 0)  || (y < 0)
            || (x >= kSize) || (y >= kSize)) {
        qDebug() <<"Wrong position!Cell wasn't set";
        qDebug() << "X :" << x <<" Y: " << y;
    } else {
        qDebug()<<"Cell was setted!Cell number:["<<x<<"]["<<y<<"]";
        mAllCells[x][y] = cell;
    }
}

//return cell value on choosed position
ECell CField::getCell(int x, int y)
{
    if ( (x < 0)  || (y < 0)
            || (x >= kSize) || (y >= kSize)) {
        qDebug()<<"Wrong place";
        qDebug() << "X :" << x <<" Y: " << y;
        return kOutofField;
    } else {
        return mAllCells[x][y];
    }
}

//get placement of ships in boolean form
QVector<QVector<bool> > CField::getPlacement()
{
    return mAllCellsBooleanForm;
}

