#ifndef CONSTANTS_H
#define CONSTANTS_H


const int kSize = 10;
const int kDefaultTimeValue = 100;
const QString kDefaultAddress = "127.0.0.1";
const qint16 kDefaultPortNumber = 1234;
const int kFieldWidth = 203;
const int kFieldHeight = 200;
const int kFieldTop = 47;
const int kFieldUserLeft = 28;
const int kFieldOpponentLeft = 262;



enum ECell
{
    kOutofField = -1,
    kEmpty = 0,
    kShip,
    kKilled,
    kInjured,
    kDot,

};

enum ECurrentState
{
    kPlacingShips = 0,
    kMakingStep,
    kWaitingForEnemy,
    kWaitingForMove,
    kWaitingForResultofMove,
    kGameEnd
};

enum ECurrentField
{
    kPlayerField = 100,
    kEnemyField = 101

};

enum EGameMode
{
    kRealOpponent = 1000,
    kComputerOpponent = 2000
};

#endif // CONSTANTS_H
