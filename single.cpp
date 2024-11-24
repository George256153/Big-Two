#include "single.h"

Single::Single(const QVector<Card> &cards)
    : CardCombonation(cards, CombType::Single)
{
    m_comparator = cards[0];
}

//QString Single::desc()
//{
//    return "Single (" + CardCombonation::desc() + ")";
//}
