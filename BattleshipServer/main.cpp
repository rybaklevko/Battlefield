#include <QCoreApplication>
#include "cserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CServer server;

    return a.exec();
}
