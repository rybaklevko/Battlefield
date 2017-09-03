#include "cmainwindow.h"
#include "ui_cmainwindow.h"
#include "cpictures.h"


//constructor
//make initialization of player/enemy field,download pictures
CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    mPicture = new CPictures;
    mPicture->isLoaded(); //load all pictures needed for battlefield
    this->update();

    mModel = new CGameModel();
    mController = new CGameController(mModel);
    connect(mController,SIGNAL(signalStateChanged()),this,SLOT(reUptade()));

    this->update();
    mModel->setState(kPlacingShips);
}

CMainWindow::~CMainWindow()
{
    delete mPicture;
    delete mModel;
    delete mController;
    delete ui;
}

//return image and set type of current field
QImage CMainWindow::getPlayerFieldImage()
{
    mWorkingField = kPlayerField;
    return reDrawCells();
}


QImage CMainWindow::getOpponentFieldImage()
{
    mWorkingField = kEnemyField;
    return reDrawCells();
}

//function,which redraw all cells
//and return image of it
QImage CMainWindow::reDrawCells()
{
    double cellx = 1.0 * kFieldWidth / double(kSize);
    double celly = 1.0 * kFieldHeight / double(kSize);

    QImage image(kFieldWidth ,
                 kFieldHeight, QImage::Format_ARGB32);
    image.fill(0);
    QPainter painter(&image);

    for (int i(0); i < kSize; i++) {
        for (int j(0); j < kSize ; j++) {
            ECell cell = mModel->getCell(i,j,mWorkingField);
            switch (cell) {
            case kDot:
                painter.drawImage(cellx*i,celly*j,
                                      mPicture->getPicture("miss"));
                break;
            case kShip:
                painter.drawImage(cellx*i,celly*j,
                                      mPicture->getPicture("score"));
                break;
            case kInjured:
                painter.drawImage(cellx*i,celly*j,
                                      mPicture->getPicture("injured"));
                break;
            case kKilled:
                painter.drawImage(cellx*i,celly*j,
                                      mPicture->getPicture("killed"));
                break;
            default:
                break;
            }
        }
    }
    return image;
}

//use label to describe current state for user
void CMainWindow::showCurrentState(const QString &data)
{
    ui->label->setText(data);
}

//slot,which update current image
//uses with signal from controller
void CMainWindow::reUptade()
{
    this->update();
}

//paint background and fields
void CMainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,this->menuBar()->geometry().height(),
                      mPicture->getPicture("background"));//draw battleground background
    painter.drawImage(kFieldUserLeft,this->menuBar()->geometry().height()
                      + kFieldTop,getPlayerFieldImage());//draw user field
    painter.drawImage(kFieldOpponentLeft,this->menuBar()->geometry().height()
                      + kFieldTop,getOpponentFieldImage());//draw enemy field
    this->update();
    switch (mModel->getState()) {
    case kPlacingShips:
        showCurrentState("Placing ships");
        break;
    case kWaitingForEnemy:
        showCurrentState("Waiting for enemy...");
        break;
    case kWaitingForMove:
        showCurrentState("Wait until enemy make move...");
        break;
    case kWaitingForResultofMove:
        showCurrentState("Wait for move result...");
        break;
    case kMakingStep:
        showCurrentState("Make your move!");
        break;
    case kGameEnd:
        showCurrentState("Game over!");
    default:
        break;
    }

}

//make action after mouse press
void CMainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint position = event->pos();
    position.setY(position.y() - this->menuBar()->geometry().height());

    if (event->button() == Qt::RightButton) {
        mController->mouseRightButtonClicked(position);
    } else {
        mController->mouseLeftButtonClicked(position);
    }
}

//after button was clicked try to connect to server
void CMainWindow::on_actionreal_player_triggered()
{
    if(!(mController->makeConnection())){
        qDebug()<<"Error with ship placement!";
    } else {
        mController->setGameMode(kRealOpponent);
        mController->sendPlacement();

        ui->actionRandom_placement->setDisabled(true);
        ui->menuStart_with->setDisabled(true);
        mModel->setState(kWaitingForEnemy);
    }
}

//after game rules button was cliecked
//shows rules of game
void CMainWindow::on_actionGame_rules_triggered()
{
    QMessageBox messageBox("Rules",
                           " This is classical battleship game."
                           " You can play against real player or computer. "
                           " Before begin game you should place your ships on battlefield. "
                           " It has to be one big ship (4-cell size), two 3-size, three 2-size and four smallest(1-cell)"
                           " Also, you can choose random placement of these ships",
                           QMessageBox::Information, QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    messageBox.exec();
}

//place all ships in random positions
void CMainWindow::on_actionRandom_placement_triggered()
{
    mController->randomFields();
}

void CMainWindow::on_actioncomputer_triggered()
{
    mController->setGameMode(kComputerOpponent);
    if(!mController->startGame()) {
        qDebug()<<"Game wasn't begin";
        return;
    }

    ui->actionRandom_placement->setDisabled(true);
    ui->menuStart_with->setDisabled(true);
}
