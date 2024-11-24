#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVector>
#include "card.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    void paint(QPainter& painter);
    void sortCards();
    void mousePress(QPoint pos);
    void playCard();
    void moveCards(qreal top, qreal left);

    QList<Card> cards;
    bool isChoosingCards;
    bool passCurrentTurn;
    int numRemainingCards;
    int score;
public slots:
    void removeCards(QVector<int> removedIndices);

signals:
//    void cardsPlayed(QVector<Card *> selectedCards);
    void cardsPlayed(QVector<int> selectedCardIndices);
//    void noCards();

};

#endif // PLAYER_H
