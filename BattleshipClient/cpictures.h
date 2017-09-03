#ifndef CPICTURES_H
#define CPICTURES_H

#include <QMap>
#include <QImage>
#include <QString>
#include <QDebug>
#include <QPainter>
#include "constants.h"


class CPictures
{
public:
    bool isLoaded();
    QImage& getPicture(const QString & name );
private:
    QMap<QString,QImage>mPictureList;
};

#endif // CPICTURES_H
