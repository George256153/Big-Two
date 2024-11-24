#include "fullhouse.h"

FullHouse::FullHouse(const QVector<Card> &cards, const Card &comparator)
    : CardCombonation(cards, CombType::FullHouse)
{
    m_comparator = comparator;
}
