#include "fourofakind.h"

FourOfAKind::FourOfAKind(const QVector<Card> &cards, const Card &comparator)
    : CardCombonation(cards, CombType::FourOfAKind)
{
    m_comparator = comparator;
}
