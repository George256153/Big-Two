#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QTextEdit>
#include <QTextCursor>
#include <QLabel>
#include <QTimer>

#include "constants.h"

#include "card.h"
#include "cardcombonation.h"
#include "single.h"
#include "pair.h"
#include "straight.h"
#include "fourofakind.h"
#include "fullhouse.h"

#include "player.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int numPlayers;
    int lastPlayIdx; // index of the player that plays the lastest play
    int curPlayerIdx; // index of current player
    

public slots:
    void checkCardCombination(QVector<int> selectedCardIndices);
    void playCard();
    void pass();
    void roundOver();
    void gameStart();
    void nextRound();
    void backToMenu();
    void onCountDownTimeOut();
    void gameExit();

protected:
    void timerEvent(QTimerEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    void distributeCards();
//    void setPrevComb(QVector<int> selectedCardIndices);
    void setPrevComb(const CardCombonation &newCardComb);
    void setPrevComb(QVector<Card> &cards); // to be removed
    void nextPlayer();

    void initFirstPlayer();
    bool isFirstPlayValid(const QVector<int> &cards);

    void initPlayingScence();
    void resetVisibility();

//    Card* card;

    QList<Card *>           pokerCards;
    QVector<Player *>       players;

    Player                  *curPlayer;
    CardCombonation         *prevComb;

    QPushButton             *playCardBtn;
    QPushButton             *passBtn;
    QPushButton             *startGameBtn;
    QPushButton             *nextRoundBtn;
    QPushButton             *backToMenuBtn;
    QPushButton             *exitBtn;

    QTextEdit               *gameReporter;
    QTextCursor             *gameReportCursor;
    QLabel                  *labelNotification;
    QLabel                  *userInfo;
    QLabel                  *userScore;

    QPixmap                 *backgroundImage;

    QTextEdit               *editNumPlayer;

    bool                    isFirstPlay;

    enum GameStatus {
        ShowMenu,
        Playing,
        Summary,
        Gameover
    };

    GameStatus              gameStatus;

    QVector<QPointF> PLAYER_POSITION = {
        QPointF(WINDOW_WIDTH - 270,         WINDOW_HEIGHT / 2 - 200),
        QPointF(WINDOW_WIDTH / 2 + 150,     50),
        QPointF(RIGHT_VIEW_X,               WINDOW_HEIGHT / 2 - 200),
    };

    QTimer                  *countDownTimer;
    int                     countDown;
};
#endif // MAINWINDOW_H
