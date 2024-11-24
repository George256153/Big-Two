#ifndef SINGLE_H
#define SINGLE_H

#include "cardcombonation.h"

class Single : public CardCombonation
{
public:
    Single(const QVector<Card>& cards);

//    QString desc() override;
};

#endif // SINGLE_H
