#ifndef CFIELD_H
#define CFIELD_H

#include <QVector>
#include <QPoint>
#include <QDebug>
#include "constants.h"


class CField
{
public:
    CField();
    void clearField();
    void setCell(int x,int y,ECell cell);
    void setPlacement(const QVector<QVector<bool> > &allcell);
    ECell getCell(int x,int y);
    QPoint changeCoordinatesType(int x, int y, ECurrentField field);
    bool isCorrectShipPlace(int x,int y);
    QVector<QVector<bool>> getPlacement();
private:
    QVector<QVector<ECell>>mAllCells;
    QVector<QVector<bool>>mAllCellsBooleanForm;
};

#endif // CFIELD_H
