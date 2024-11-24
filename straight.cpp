#include "straight.h"

Straight::Straight(const QVector<Card> &cards)
    : CardCombonation(cards, CombType::Straight)
{
    m_comparator = (cards[0].m_number == 2) ? cards[0] : cards[4];
}
