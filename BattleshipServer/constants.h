#ifndef CONSTANTS_H
#define CONSTANTS_H

#define _GLIBCXX_USE_NANOSLEEP

#include <QHostAddress>

const int kSize = 10;
const int kDefaultTimeValue = 100;
const QHostAddress kDefaultAddress = QHostAddress::Any;
const qint16 kDefaultPortNumber = 1234;


enum ECell
{
    kOutofField = -1,
    kEmpty = 0,
    kShip,
    kKilled,
    kInjured,
    kDot,

};

enum ECurrentClientState
{
    kMakingStep,
    kWaitingForEnemyStep,
    kWaitingForEnemyConnection
};
#endif // CONSTANTS_H
