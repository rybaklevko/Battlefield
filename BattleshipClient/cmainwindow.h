#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <Qpainter>
#include "cpictures.h"
#include "cgamemodel.h"
#include "cgamecontroller.h"

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();
    QImage getPlayerFieldImage();
    QImage getOpponentFieldImage();
    QImage reDrawCells();
    void parseData(QString data);
    void enemyMove();
    void enemyResult(ECell state);
    void enemyPlacement();
    std::string serverMessage();

    void showCurrentState(const QString &data);
public slots:
    void reUptade();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * event);
private slots:
    void on_actionreal_player_triggered();

    void on_actionGame_rules_triggered();

    void on_actionRandom_placement_triggered();

    void on_actioncomputer_triggered();

private:
    Ui::CMainWindow *ui;
    CPictures *mPicture;
    //ECurrentState mState;
    ECurrentField mWorkingField;
    std::string mReceivedData;
    CGameModel *mModel;
    CGameController *mController;
};

#endif // CMAINWINDOW_H
