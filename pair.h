#ifndef PAIR_H
#define PAIR_H

#include "cardcombonation.h"

class Pair : public CardCombonation
{
public:
    Pair(const QVector<Card>& cards);

//    bool operator <(const CardCombonation &compared) override;
//    QString desc() override;
};

#endif // PAIR_H
