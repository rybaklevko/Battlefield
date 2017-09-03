#ifndef CFIELD_H
#define CFIELD_H

#include <QVector>
#include <QDebug>
#include "constants.h"


class CField
{
public:
    void cellInitializatiion(QString & data);
    void setCell(int x,int y,ECell cell);
    ECell getCell(int x,int y);
    QVector<QVector<bool>> getPlacement();
private:
    QVector<QVector<ECell>>mAllCells;
    QVector<QVector<bool>>mAllCellsBooleanForm;
};

#endif // CFIELD_H
