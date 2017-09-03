#ifndef CCOMPUTEROPPONENT_H
#define CCOMPUTEROPPONENT_H

#include "cgamemodel.h"
#include "constants.h"

class CComputerOpponent
{
public:
    CComputerOpponent(CGameModel *computermodel);
    void generatePlacement();
    QPoint generateMove();
private:
    CGameModel *mComputerModel;
};

#endif // CCOMPUTEROPPONENT_H
