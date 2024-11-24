#ifndef CARD_H
#define CARD_H

#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QPoint>
#include "constants.h"

class Card : public QRectF
{
public:
    Card();
    Card(Suit suit, int number);
    Card(const Card &anotherCard);
    ~Card();

    /* descriptions */
    QString descSuit();
    QString descNumber();
    QString desc();

    QString getCardImagePath(Suit suit, int number);

    bool        operator<(const Card &compared) const;
    void        operator=(const Card &anotherCard);

    void        paint(QPainter& painter);
    bool        checkMousePress(const QPoint& pos);

    Suit        m_suit;
    int         m_number;
    QPixmap     m_image;
    bool        b_selected;
    bool        b_played;
};

#endif // CARD_H
