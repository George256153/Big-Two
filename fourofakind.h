#ifndef FOUROFAKIND_H
#define FOUROFAKIND_H

#include "cardcombonation.h"

class FourOfAKind : public CardCombonation
{
public:
    FourOfAKind(const QVector<Card> &cards, const Card &comparator);
};

#endif // FOUROFAKIND_H
