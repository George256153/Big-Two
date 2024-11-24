#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , numPlayers(4)
    , lastPlayIdx(-1)
    , curPlayer(nullptr)
    , prevComb(nullptr)
    , playCardBtn(nullptr)
    , passBtn(nullptr), startGameBtn(nullptr), nextRoundBtn(nullptr), backToMenuBtn(nullptr)//,exitBtn(nullptr)
    , gameReporter(nullptr), gameReportCursor(nullptr)
    , labelNotification(nullptr), userInfo(nullptr), userScore(nullptr), editNumPlayer(nullptr)
    , isFirstPlay(true)
    , gameStatus(GameStatus::ShowMenu)
    , countDown(COUNTDOWN_TIMES)
    , countDownTimer(nullptr)
{
    setWindowTitle("Big Two");
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
    

    backgroundImage = new QPixmap(":/Image/bg.jpg");

    // init menu
    editNumPlayer = new QTextEdit(this);
    editNumPlayer->setPlaceholderText("Please enter the number of players (2~4)");
    editNumPlayer->setFont(QFont("times", 16));
    editNumPlayer->setFixedSize(700, 60);
    editNumPlayer->move((WINDOW_WIDTH - editNumPlayer->width()) / 2,
                        150 + backgroundImage->height());

    startGameBtn = new QPushButton("Game Start", this);
    startGameBtn->setFixedHeight(editNumPlayer->height());
    startGameBtn->move(editNumPlayer->x() + editNumPlayer->width() + 10,
                       editNumPlayer->y());

    connect(startGameBtn, &QPushButton::clicked, this, &MainWindow::gameStart);

    exitBtn = new QPushButton("Exit", this);
    exitBtn->move(editNumPlayer->x() + editNumPlayer->width() + 120,
                    editNumPlayer->y()+15);
    connect(exitBtn, &QPushButton::clicked, this, &MainWindow::gameExit);


    countDownTimer = new QTimer(this);
    connect(countDownTimer, &QTimer::timeout, this, &MainWindow::onCountDownTimeOut);



}

MainWindow::~MainWindow()
{
    while (pokerCards.size() > 0)
    {
        Card* tmp = pokerCards.back();
        pokerCards.pop_back();
        delete tmp;
        tmp = nullptr;
    }

    if (prevComb)
    {
        delete prevComb;
        prevComb = nullptr;
    }

    if (gameReportCursor)
    {
        delete gameReportCursor;
        gameReportCursor = nullptr;
    }

    if (backgroundImage)
    {
        delete backgroundImage;
        backgroundImage = nullptr;
    }
}

void MainWindow::playCard()
{
    if (curPlayer) curPlayer->playCard();
}

void MainWindow::pass()
{
    if (!prevComb)
    {
        //qDebug() << "Cannot pass";
        labelNotification->setText("Sorry, you cannot pass. It's your turn and you can play any cards.");
    }
    else
    {
        curPlayer->passCurrentTurn = true;

        gameReporter->append(QString("Player %1 decides to pass. ").arg(curPlayerIdx));
        nextPlayer();
        gameReporter->append(QString(" -> Now, it's Player %1's turn.\n").arg(curPlayerIdx));

        labelNotification->setText("Please choose your cards.");

        update();
    }
}

void MainWindow::roundOver()
{    
    // 0. clear all cards & reset the desk
    for (int i = 0; i < players.size(); i++)
    {
        players[i]->moveCards(130 + (CARD_HEIGHT + 40) * i, RIGHT_VIEW_X);
        players[i]->passCurrentTurn = false;
    }

    // 1. report round over
    QString winMessage = QString("Player %1 wins this round!!!\n").arg(curPlayerIdx);
//    labelNotification->setText("");
//    userInfo->setText("");

    gameReporter->append("[Game Summary]");
    gameReporter->append(" -> " + winMessage);

    int score = 0;

    for (int i = 0; i < players.size(); ++i)
    {
        if (i != curPlayerIdx)
        {
            bool hasTwoInRemainingCards = false;
            for (auto it = players[i]->cards.rbegin(); it != players[i]->cards.rend(); ++it)
            {
                if (!it->b_played)
                {
                    hasTwoInRemainingCards = (it->m_number == 2);
                    break;
                }
            }
            if (hasTwoInRemainingCards)
                score += players[i]->numRemainingCards * 2;
            else
                score += players[i]->numRemainingCards;

            gameReporter->append(QString(" -> Player %1 : %2 cards remain.")
                                     .arg(i)
                                     .arg(players[i]->numRemainingCards)
                                 + (hasTwoInRemainingCards ? " (x2)" : "")
                                );
        }
    }
    curPlayer->score += score;

    gameReporter->append("");
    gameReporter->append(QString("=> Player %1 gets %2 scores from this round.").arg(curPlayerIdx).arg(score));
    gameReporter->append(QString("=> Now, Player %1 has %2 scores.").arg(curPlayerIdx).arg(curPlayer->score));

    // 3. check if game over or next round
    if (curPlayer->score >= 50)
    {
        gameStatus = GameStatus::Gameover;
    }
    else
    {
        gameStatus = GameStatus::Summary;
    }
    countDownTimer->stop();
    resetVisibility();
    update();
}

void MainWindow::checkCardCombination(QVector<int> cardIndices)
{
    if (DEBUG)
    {
        lastPlayIdx = curPlayerIdx;
        curPlayer->removeCards(cardIndices);
        if (curPlayer->numRemainingCards != 0) nextPlayer();
        else roundOver();

        update();
        return;
    }

    if (isFirstPlay && !isFirstPlayValid(cardIndices))
    {
        labelNotification->setText("Sorry, the first play must contains Club-3.");
        return;
    }

    /* get selected cards */
    QVector<Card> selectedCards;
    for (const int &idx : cardIndices)
    {
        selectedCards << curPlayer->cards[idx];
    }

    /* check if selected cards is a valid combination */
    CardCombonation *newCardComb = nullptr;

    switch (cardIndices.size()) {
    case 1:
        //qDebug() << "Single";
        newCardComb = new Single(selectedCards);
        break;
    case 2:
        if ( curPlayer->cards[cardIndices[0]].m_number == curPlayer->cards[cardIndices[1]].m_number )
        {
            //qDebug() << "Pair";
            newCardComb = new Pair(selectedCards);
        }
        else
        {
            //qDebug() << "2 cards";
        }
        break;
    case 5:
        // check Four-of-a-kind
        if (curPlayer->cards[cardIndices[0]].m_number == curPlayer->cards[cardIndices[1]].m_number
                && curPlayer->cards[cardIndices[1]].m_number == curPlayer->cards[cardIndices[2]].m_number
                && curPlayer->cards[cardIndices[2]].m_number == curPlayer->cards[cardIndices[3]].m_number)
        {
            //qDebug() << "Four of a kind";
            newCardComb = new FourOfAKind(selectedCards, selectedCards[3]);
            break;
        }
        else if (curPlayer->cards[cardIndices[1]].m_number == curPlayer->cards[cardIndices[2]].m_number
                && curPlayer->cards[cardIndices[2]].m_number == curPlayer->cards[cardIndices[3]].m_number
                && curPlayer->cards[cardIndices[3]].m_number == curPlayer->cards[cardIndices[4]].m_number)
        {
            //qDebug() << "Four of a kind";
            newCardComb = new FourOfAKind(selectedCards, selectedCards[4]);
            break;
        }
        // check full-house (0=1=2 && 3=4)
        else if (curPlayer->cards[cardIndices[0]].m_number == curPlayer->cards[cardIndices[1]].m_number
                && curPlayer->cards[cardIndices[1]].m_number == curPlayer->cards[cardIndices[2]].m_number
                && curPlayer->cards[cardIndices[3]].m_number == curPlayer->cards[cardIndices[4]].m_number)
        {
            //qDebug() << "full-house";
            newCardComb = new FullHouse(selectedCards, selectedCards[2]);
            break;
        }
        // check full-house (0=1 && 2=3=4)
        else if (curPlayer->cards[cardIndices[2]].m_number == curPlayer->cards[cardIndices[3]].m_number
                 && curPlayer->cards[cardIndices[3]].m_number == curPlayer->cards[cardIndices[4]].m_number
                 && curPlayer->cards[cardIndices[0]].m_number == curPlayer->cards[cardIndices[1]].m_number)
        {
            //qDebug() << "full-house";
            newCardComb = new FullHouse(selectedCards, selectedCards[4]);
            break;
        }
        // check straight
        else if ((curPlayer->cards[cardIndices[4]].m_number - curPlayer->cards[cardIndices[3]].m_number == 1
                    && curPlayer->cards[cardIndices[3]].m_number - curPlayer->cards[cardIndices[2]].m_number == 1
                    && curPlayer->cards[cardIndices[2]].m_number - curPlayer->cards[cardIndices[1]].m_number == 1
                    && curPlayer->cards[cardIndices[1]].m_number - curPlayer->cards[cardIndices[0]].m_number == 1)
                 || /* 10, J, Q, K, A */
                 (curPlayer->cards[cardIndices[0]].m_number == 10
                  && curPlayer->cards[cardIndices[1]].m_number == 11 && curPlayer->cards[cardIndices[2]].m_number == 12
                  && curPlayer->cards[cardIndices[3]].m_number == 13 && curPlayer->cards[cardIndices[4]].m_number == 1)
                 || /* 3, 4, 5, 6, 2 */
                 (curPlayer->cards[cardIndices[0]].m_number == 3
                  && curPlayer->cards[cardIndices[1]].m_number == 4 && curPlayer->cards[cardIndices[2]].m_number == 5
                  && curPlayer->cards[cardIndices[3]].m_number == 6 && curPlayer->cards[cardIndices[4]].m_number == 2))
        {
            // check straight-flush
            if (curPlayer->cards[cardIndices[4]].m_suit == curPlayer->cards[cardIndices[3]].m_suit
                && curPlayer->cards[cardIndices[3]].m_suit == curPlayer->cards[cardIndices[2]].m_suit
                && curPlayer->cards[cardIndices[2]].m_suit == curPlayer->cards[cardIndices[1]].m_suit
                && curPlayer->cards[cardIndices[1]].m_suit == curPlayer->cards[cardIndices[0]].m_suit)
            {
                //qDebug() << "full straight";
                newCardComb = new Straight(selectedCards);
                newCardComb->m_combType = CardCombonation::CombType::StraightFlush;
            }
            else
            {
                //qDebug() << "straight";
                newCardComb = new Straight(selectedCards);
            }
            break;
        }
        else
        {
            //qDebug() << "5 cards";
            break;
        }
    default:
        labelNotification->setText( QString("Invalid number of cards: %1").arg(cardIndices.size()) );
        return;
    }

    if (!newCardComb)
    {
        labelNotification->setText("Invalid card combination :(");
        return;
    }

    if (prevComb && !(*prevComb < *newCardComb))
    {
        labelNotification->setText("Incomparable or smaller rank than the previous play.");
    }
    else
    {
        gameReporter->append(QString("[Player %1] ").arg(curPlayerIdx) + newCardComb->desc() + " is played");
        gameReporter->append(QString(" -> Player %1 has %2 cards now.\n")
                                 .arg(curPlayerIdx)
                                 .arg(curPlayer->numRemainingCards - newCardComb->m_cards.size()));

        prevComb = newCardComb;

        lastPlayIdx = curPlayerIdx;
        curPlayer->removeCards(cardIndices);
        if (curPlayer->numRemainingCards != 0) nextPlayer();
        else roundOver();

        update();
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    /* Painter setup */
    QPainter painter(this);

    QPen pen;
    pen.setColor(Qt::black);

    QFont font;
    font.setPixelSize(20);
    font.setBold(true);

    painter.setPen(pen);
    painter.setFont(font);

    if (gameStatus == GameStatus::ShowMenu)
    {
        painter.drawPixmap((WINDOW_WIDTH - backgroundImage->width()) / 2,
                           100,
                           *backgroundImage);
    }
    else if (gameStatus == GameStatus::Playing)
    {
        if (curPlayer) curPlayer->paint(painter);
        if (prevComb) prevComb->paint(painter);

        int otherPlayerIdx = curPlayerIdx;
        for (int i = 0; i < players.size() - 1; ++i)
        {
            if (++otherPlayerIdx == numPlayers) otherPlayerIdx = 0;
            painter.drawText(PLAYER_POSITION[i],
                             QString("Player %1 (%2/%3)")
                                 .arg(otherPlayerIdx)
                                 .arg(players[otherPlayerIdx]->numRemainingCards)
                                 .arg(players[otherPlayerIdx]->cards.size())
                             );
            painter.drawText(PLAYER_POSITION[i] + QPointF(0, 25),
                             QString("Score: %1")
                                 .arg(players[otherPlayerIdx]->score)
                             );

            if (players[otherPlayerIdx]->passCurrentTurn)
            {
                painter.drawPixmap(PLAYER_POSITION[i] + QPointF(30, 50),
                                   QPixmap(":/Image/pass.png").scaled(150, 150, Qt::KeepAspectRatio));
            }
        }

        painter.drawText(COUNTDOWN_DISPLAY_X, COUNTDOWN_DISPLAY_Y, QString("%1").arg(countDown));
        painter.drawPixmap(COUNTDOWN_DISPLAY_X - 18, COUNTDOWN_DISPLAY_Y - 80,
                           QPixmap(":/Image/clock.png").scaled(50, 50, Qt::KeepAspectRatio));
    }
    else if (gameStatus == GameStatus::Summary || gameStatus == GameStatus::Gameover)
    {
        for (int i = 0; i < players.size(); ++i)
        {
            painter.drawText(RIGHT_VIEW_X,
                             players[i]->cards[0].top() - 15,
                             QString("Player %1 (%2 / %3) | Score: %4")
                                 .arg(i)
                                 .arg(players[i]->numRemainingCards)
                                 .arg(players[i]->cards.size())
                                 .arg(players[i]->score)
                                 + (curPlayer == players[i] ? " <-- Winner" : "")
                             );
            players[i]->paint(painter);
        }
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (curPlayer)
        curPlayer->mousePress(event->pos());

    update();
}

void MainWindow::distributeCards()
{
    QVector<int> indices(pokerCards.size());
    for (int i = 0; i < indices.size(); ++i)
    {
        indices[i] = i;
    }
    int index1 = 0;
    int index2 = 0;
    int tmp = 0;
    srand(time(NULL));
    for (int cnt = 0; cnt < 500; ++cnt)
    {
        index1 = rand() % pokerCards.size();
        index2 = rand() % pokerCards.size();
        tmp = indices[index1];
        indices[index1] = indices[index2];
        indices[index2] = tmp;
    }

    int numCardsPerPlayer = indices.size() / numPlayers;
    for (int playerIdx = 0; playerIdx < numPlayers; ++playerIdx)
    {
        for (int i = numCardsPerPlayer * playerIdx;
             i < numCardsPerPlayer * (playerIdx + 1);
             ++i)
        {
            players[playerIdx]->cards << *(pokerCards[indices[i]]);
        }

        //qDebug() << "player" << playerIdx << "has" << players[playerIdx]->cards.size() << "cards.";
    }

    if (numPlayers == 3
        && pokerCards.back()->m_suit == Suit::Clubs
        && pokerCards.back()->m_number == 3)
    {
        players[int(rand()%3)]->cards << *pokerCards.back();
    }
}

void MainWindow::setPrevComb(const CardCombonation &newCardComb)
{
    if (!prevComb)
    {
        prevComb = new CardCombonation(newCardComb.m_cards, newCardComb.m_combType);
    }
    prevComb->m_comparator = newCardComb.m_comparator;
}


void MainWindow::setPrevComb(QVector<Card> &cards)
{
    if (!prevComb)
    {
        prevComb = new CardCombonation(cards, CardCombonation::CombType::Single);
    }
}

void MainWindow::nextPlayer()
{
    do {
        if (++curPlayerIdx == numPlayers)
        {
            curPlayerIdx = 0;
        }
        curPlayer = players[curPlayerIdx];
    } while (curPlayer->passCurrentTurn);

    if (curPlayerIdx == lastPlayIdx)
    {
        if (prevComb)
        {
            delete prevComb;
            prevComb = nullptr;

            foreach (Player* player, players)
            {
                player->passCurrentTurn = false;
            }

            update();
        }
        else
        {
            //qDebug() << "Error";
        }
    }

    userInfo->setText(QString("[Player %1] (%2, %3)")
                          .arg(curPlayerIdx)
                          .arg(curPlayer->numRemainingCards)
                          .arg(curPlayer->cards.size()));
    userScore->setText(QString("Score: %1").arg(curPlayer->score));
    labelNotification->setText("Please choose your cards.");

    countDown = COUNTDOWN_TIMES;
}

void MainWindow::initFirstPlayer()
{
    for (int i = 0; i < numPlayers; ++i)
    {
        if (players[i]->cards[0].m_suit == Suit::Clubs
            && players[i]->cards[0].m_number == 3)
        {
            curPlayerIdx = i;
            curPlayer = players[i];
        }
    }

    if (numPlayers == 3)
    {
        if ( !(pokerCards.back()->m_suit == Suit::Clubs && pokerCards.back()->m_number == 3) )
        {
            curPlayer->cards << *pokerCards.back();
            curPlayer->sortCards();
            curPlayer->moveCards(CARD_TOP, RIGHT_VIEW_X);
            curPlayer->numRemainingCards = curPlayer->cards.size();
        }
    }
}

bool MainWindow::isFirstPlayValid(const QVector<int> &indices)
{
    Card *smallestCard = &(curPlayer->cards[indices[0]]);
    if (smallestCard->m_suit == Suit::Clubs && smallestCard->m_number == 3)
    {
        isFirstPlay = false;
        return true;
    }
    return false;
}

void MainWindow::gameStart()
{
    QString userInput = editNumPlayer->toPlainText();

    if (userInput.toInt() >= 2 && userInput.toInt() <= 4)
    {
        numPlayers = userInput.toInt();
        initPlayingScence();
        nextRound();
    }
}

void MainWindow::nextRound()
{
    if (prevComb)
    {
        delete prevComb;
        prevComb = nullptr;
    }

    foreach (auto player, players)
    {
        player->cards.clear();
    }

    // distribute cards to all players
    //qDebug() << "[Begin] distributeCards";
    distributeCards();
    //qDebug() << "[End] distributeCards";

    for (int i = 0; i < players.size(); ++i)
    {
        //qDebug() << "[Begin] sort cards" << i;
        players[i]->sortCards();
        //qDebug() << "[Begin] move cards" << i;
        players[i]->moveCards(CARD_TOP, RIGHT_VIEW_X);
        //qDebug() << "[Begin] get remaining card" << i;
        players[i]->numRemainingCards = players[i]->cards.size();
    }

    //qDebug() << "[Begin] init first player";
    initFirstPlayer();
    //qDebug() << "[End] init first player";

    userInfo->setText(QString("[Player %1] (%2, %3)")
                          .arg(curPlayerIdx)
                          .arg(curPlayer->numRemainingCards)
                          .arg(curPlayer->cards.size()));
    userScore->setText(QString("Score: %1").arg(curPlayer->score));

    gameReporter->append(QString("[Game Starts]\n"
                                 " -> First player: Player %1 !\n").arg(curPlayerIdx));

    labelNotification->setText("Please choose your cards.");

    gameStatus = GameStatus::Playing;
    resetVisibility();
    update();

    countDown = COUNTDOWN_TIMES;
    countDownTimer->start(COUNTDOWN_INTERVAL);
}

void MainWindow::backToMenu()
{
    //qDebug() << "back to menu";
    for (int i = 0; i < players.size(); ++i)
    {
        Player **player = &players[i];
        if (*player)
        {
            (*player)->deleteLater();
            *player = nullptr;
        }
    }

    countDownTimer->stop();

    curPlayer = nullptr;

    if (prevComb)
    {
        delete prevComb;
        prevComb = nullptr;
    }

    players.clear();

    gameStatus = GameStatus::ShowMenu;
    resetVisibility();
    update();
}

void MainWindow::onCountDownTimeOut()
{
    if (--countDown == 0)
    {
        if (prevComb)          // pass if there exists previous combination
        {
            pass();
        }
        else                    // play the left most card
        {
            int playedCardIndex = 0;
            for (; curPlayer && playedCardIndex < curPlayer->cards.size(); ++playedCardIndex)
            {
                if (curPlayer && !curPlayer->cards[playedCardIndex].b_played)
                {
                    break;
                }
            }
            QVector<int> playedCards{playedCardIndex};
            if (curPlayer) emit curPlayer->cardsPlayed(playedCards);
        }
    }
    update();
}

void MainWindow::initPlayingScence()
{
    // show all poker cards
    if (pokerCards.size() == 0)
    {
        for (int i = 1; i <= 13; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                pokerCards << new Card(Suit(j), i);
            }
        }
    }

    // init players
    for (int i = 0; i < numPlayers; ++i)
    {
        players.append(new Player);
        connect(players.back(), &Player::cardsPlayed,   this, &MainWindow::checkCardCombination);
    }

    if (!gameReporter)
    {
        gameReporter = new QTextEdit(this);
        gameReporter->move(GAME_REPORTER_X, GAME_REPORTER_Y);
        gameReporter->setFixedSize(GAME_REPORTER_WIDTH, GAME_REPORTER_HEIGHT);
        gameReporter->setReadOnly(true);
        gameReporter->setFont(QFont("times", 12));
        gameReporter->setLineWrapMode(QTextEdit::FixedPixelWidth);
        gameReporter->setLineWrapColumnOrWidth(GAME_REPORTER_WIDTH + 100);
    }
    gameReporter->setPlainText("");

    QFont labelFont;
    labelFont.setPixelSize(20);
    labelFont.setBold(true);

    if (!labelNotification)
    {
        labelNotification = new QLabel(this);
        labelNotification->setFont(labelFont);
        labelNotification->setStyleSheet("QLabel {color: blue;}");
        labelNotification->setFixedWidth(WINDOW_WIDTH/2);
        labelNotification->setFixedHeight(50);
        labelNotification->move(RIGHT_VIEW_X + 250, 550);
    }
    labelNotification->setText("Please choose your cards.");

    if (!userInfo)
    {
        userInfo = new QLabel(this);
        userInfo->setFont(labelFont);
        userInfo->setStyleSheet("QLabel {color: blue;}");
        userInfo->setFixedWidth(WINDOW_WIDTH/2);
        userInfo->setFixedHeight(50);
        userInfo->move(RIGHT_VIEW_X, 550);
    }

    if (!userScore)
    {
        userScore = new QLabel(this);
        userScore->setFont(labelFont);
        userScore->setStyleSheet("QLabel {color: blue;}");
        userScore->setFixedWidth(WINDOW_WIDTH/2);
        userScore->setFixedHeight(50);
        userScore->move(RIGHT_VIEW_X, 590);
    }


    /* Button */
    if (!playCardBtn)
    {
        playCardBtn = new QPushButton("Play Card", this);
        connect(playCardBtn, &QPushButton::clicked, this, &MainWindow::playCard);
        playCardBtn->move(RIGHT_VIEW_X, 900);
    }

    if (!passBtn)
    {
        passBtn = new QPushButton("Pass", this);
        connect(passBtn, &QPushButton::clicked, this, &MainWindow::pass);
        passBtn->move(RIGHT_VIEW_X + 150, 900);
    }

    if (!nextRoundBtn)
    {
        nextRoundBtn = new QPushButton("Next Round", this);
        connect(nextRoundBtn, &QPushButton::clicked, this, &MainWindow::nextRound);
        nextRoundBtn->move(WINDOW_WIDTH - 500, 900);
    }

    if (!backToMenuBtn)
    {
        backToMenuBtn = new QPushButton("Back to Menu", this);
        connect(backToMenuBtn, &QPushButton::clicked, this, &MainWindow::backToMenu);
        backToMenuBtn->move(50, gameReporter->y() + gameReporter->height() + 30);
        backToMenuBtn->setFixedSize(gameReporter->width(), 60);
    }
}

//bool  MainWindow::exitbotton=false;

void MainWindow::gameExit()
{
    close();
}

void MainWindow::resetVisibility()
{
    editNumPlayer->setVisible(gameStatus == ShowMenu);
    startGameBtn->setVisible(gameStatus == ShowMenu);
    exitBtn->setVisible(gameStatus == ShowMenu);

    userInfo->setVisible(gameStatus == Playing);
    userScore->setVisible(gameStatus == Playing);
    labelNotification->setVisible(gameStatus == Playing);
    playCardBtn->setVisible(gameStatus == Playing);
    passBtn->setVisible(gameStatus == Playing);

    nextRoundBtn->setVisible(gameStatus == Summary);

    gameReporter->setVisible(gameStatus != ShowMenu);
    backToMenuBtn->setVisible(gameStatus != ShowMenu);

}
