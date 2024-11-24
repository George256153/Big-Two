#include "pair.h"

//Pair::Pair(const Card &comparator)
//    : CardCombonation(comparator)
//{

//}

Pair::Pair(const QVector<Card> &cards)
    : CardCombonation(cards, CombType::Pair)
{
    m_comparator = cards[1];
}

//bool Pair::operator <(const CardCombonation &compared)
//{
//    /* except */
//    if (compared.m_combType == CombType::FourOfAKind || compared.m_combType == CombType::StraightFlush)
//    {
//        return true;
//    }

//    if (compared.m_combType != m_combType)
//        return false; // in-comparable
//    else
//        return (m_comparator < compared.m_comparator);
//}


//QString Pair::desc()
//{
//    return "Pair (" + CardCombonation::desc() + ")";
//}
