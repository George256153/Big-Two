#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "cardcombonation.h"

class Straight : public CardCombonation
{
public:    
    Straight(const QVector<Card>& cards);
};

#endif // STRAIGHT_H
