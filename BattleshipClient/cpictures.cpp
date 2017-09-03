#include "cpictures.h"

//function to load picture
//pictures are load from resource file
//each time check does current picture was downloaded
bool CPictures::isLoaded()
{
   QImage image[5];
   image[0].load(":/image/battleship.jpg");
   mPictureList.insert("background", image[0]);

   image[1].load(":/image/o.gif");
   mPictureList.insert("miss",image[1]);

   image[2].load(":/image/cell.jpg");
   mPictureList.insert("score",image[2]);

   image[3].load(":/image/xkilled.gif");
   mPictureList.insert("killed",image[3]);

   image[4].load(":/image/x.gif");
   mPictureList.insert("injured",image[4]);
   for(int i(0); i < 5; i++) {
       if (image[i].isNull()) {
           qDebug()<<"Picture loading error!";
           return false;
       }
   }
   return true;
}

//function, which seach and return picture by name
QImage &CPictures::getPicture(const QString &name)
{
    QMap<QString, QImage>::iterator i = mPictureList.find(name);

    //if picture wasn't found
    if (i == mPictureList.end()) {
        qDebug()<<"Picture wasn't found!";
        throw 1;
    }
    return i.value();
}

