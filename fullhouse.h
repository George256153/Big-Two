#ifndef FULLHOUSE_H
#define FULLHOUSE_H

#include "cardcombonation.h"

class FullHouse : public CardCombonation
{
public:
    FullHouse(const QVector<Card> &cards, const Card &comparator);
};

#endif // FULLHOUSE_H
