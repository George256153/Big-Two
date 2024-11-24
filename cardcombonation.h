#ifndef CARDCOMBONATION_H
#define CARDCOMBONATION_H

#include <QObject>
#include <QPainter>
#include <QDebug>
#include <QDebug>
#include "card.h"
#include "constants.h"

//class StraightFlush;
//class FourOfAKind;

class CardCombonation : public QObject
{
    Q_OBJECT
public:
    enum class CombType {
        Single,
        Pair,
        Straight,
        FullHouse,
        FourOfAKind,
        StraightFlush
    };

    QVector<QString>    m_descCompType = {
        "Single",
        "Pair",
        "Straight",
        "FullHouse",
        "FourOfAKind",
        "StraightFlush"
    };

    CardCombonation(const QVector<Card> &cards, CombType combType);
//    CardCombonation(const Card &comparator);

    virtual bool operator<(const CardCombonation& compared);

    virtual QString desc();

    void paint(QPainter &painter);

    CombType            m_combType;
    QVector<Card>       m_cards;

    Card                m_comparator; // use this card to compare with other card combination (which has some type)
};

#endif // CARDCOMBONATION_H
