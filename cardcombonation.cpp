#include "cardcombonation.h"

CardCombonation::CardCombonation(const QVector<Card> &cards, CombType combType)
    : m_combType(combType)
{
    for (const Card &card : cards)
    {
        m_cards << card;
    }
}

//CardCombonation::CardCombonation(const Card &comparator)
//    : comparator(comparator)
//{

//}

bool CardCombonation::operator<(const CardCombonation &compared)
{
    if (compared.m_combType == m_combType)              return (m_comparator < compared.m_comparator);
    if (compared.m_combType == CombType::StraightFlush) return true;
    if (compared.m_combType == CombType::FourOfAKind)   return (m_combType != CombType::StraightFlush);
    return false;
}

QString CardCombonation::desc()
{
    return m_descCompType[int(m_combType)] + " (" + m_comparator.desc() + ")";
}


void CardCombonation::paint(QPainter &painter)
{
    for (int i = 0; i < m_cards.size(); ++i)
    {
//        cards[i].paint(painter);
        painter.drawPixmap((WINDOW_WIDTH + RIGHT_VIEW_X) / 2 - 200 + CARD_X_INTERVAL * i,
                           WINDOW_HEIGHT / 2 - 300,
                           m_cards[i].m_image.scaled(CARD_WIDTH - 30, CARD_HEIGHT - 30, Qt::KeepAspectRatio));
    }
}
