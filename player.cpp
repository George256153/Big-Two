#include "player.h"

Player::Player(QObject *parent)
    : QObject(parent)
    , passCurrentTurn(false)
    , numRemainingCards(-1)
    , score(0)
{

}

void Player::paint(QPainter &painter)
{
    for (int i = 0; i < cards.size(); ++i)
    {
        if (!cards[i].b_played)
            cards[i].paint(painter);
    }
}

void Player::sortCards()
{
    // std::sort(cards.begin(), cards.end());
    qSort(cards);
}

void Player::mousePress(QPoint pos)
{
    for (QList<Card>::reverse_iterator it = cards.rbegin();
         it != cards.rend();
         it++)
    {
        if (it->checkMousePress(pos))
        {
            break;
        }
    }
}

void Player::playCard()
{
    QVector<int> selectedCardIndices;
    for (int i = 0; i < cards.size(); ++i)
    {
        if (cards[i].b_selected && !cards[i].b_played)
        {
            selectedCardIndices << i;
            //qDebug() << " -> card:"
                     //<< cards[i].desc();
        }
    }

    emit cardsPlayed(selectedCardIndices);
    // pass the cards to combination checker
    // if pass, emit signal for next round
    // if not pass, emit goes back to wait for clicking buttons
}

void Player::moveCards(qreal top, qreal left)
{
    for (int i = 0; i < cards.size(); ++i)
    {
        cards[i].moveTop(top);
        cards[i].moveLeft(left + i * CARD_X_INTERVAL);
    }
}

void Player::removeCards(QVector<int> removedIndices)
{
    for (int& idx : removedIndices)
    {
        cards[idx].b_played = true;
        cards[idx].moveTop(WINDOW_HEIGHT); // cannot be pressed by mouse
    }
    if (numRemainingCards > 0) numRemainingCards -= removedIndices.size();
}
