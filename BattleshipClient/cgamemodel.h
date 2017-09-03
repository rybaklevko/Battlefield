#ifndef CGAMEMODEL_H
#define CGAMEMODEL_H

#include <QObject>
#include <QTime>
#include "cfield.h"

class CGameModel : public QObject
{
    Q_OBJECT
public:
    explicit CGameModel(QObject *parent = 0);
    ~CGameModel();
    void clearField(ECurrentField field);
    ECell getCell(int x,int y, ECurrentField field);
    void setCell(int x, int y, ECell cell, ECurrentField field);
    QPoint changeCoordinatesType(int x,int y, ECurrentField field);
    QVector<QVector<bool>> getPlacement(ECurrentField field);
    void setState(ECurrentState state);
    ECurrentState getState();
    bool isCorrectShipPlacement();
    bool isCorrectShipPlace(int x, int y, ECurrentField field);
    int shipNumbers(int size);
    bool isShip(int size, int x ,int y);
    void randomShip(int size);
    void addInjured();
    int getNumberOfInjured();

signals:

public slots:


private:
    CField *mPlayerField;
    CField *mEnemyField;
    ECurrentState mState;
    int mNumberOfInjured = 0;
};

#endif // CGAMEMODEL_H
