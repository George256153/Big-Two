#include "card.h"


Card::Card()
    : m_suit(Suit::Clubs)
    , m_number(3)
    , b_selected(false)
    , b_played(false)
{

}

Card::Card(Suit suit, int number)
    : m_suit(suit)
    , m_number(number)
    , b_selected(false)
    , b_played(false)
{
    setWidth(CARD_WIDTH);
    setHeight(CARD_HEIGHT);

    QString str_suit = descSuit();
    QString str_number = descNumber();
    QString imageFilePath =  QString(":/Image/%1_of_%2.png").arg(str_number, str_suit);
    m_image = QPixmap(imageFilePath);
}

Card::Card(const Card &anotherCard)
    : Card(anotherCard.m_suit, anotherCard.m_number)
{

}

Card::~Card()
{
}

QString Card::descSuit()
{
    return SUITS[m_suit];
}

QString Card::descNumber()
{
    if (m_number >= 2 && m_number <= 10)
    {
        return QString("%1").arg(m_number);
    }
    switch (m_number) {
    case 1:
        return "ace";
    case 11:
        return "jack";
    case 12:
        return "queen";
    case 13:
        return "king";
    default:
        //qDebug() << "invaild number";
        return "";
    }
}

QString Card::desc()
{
    return descSuit() + ", " + descNumber();
}

QString Card::getCardImagePath(Suit suit, int number)
{

}

bool Card::operator<(const Card &compared) const
{
    // weighted number such that 2 > A > K > Q > ... > 4 > 3
    int weightedNum         = m_number          + (m_number <= 2)           * 13;
    int comparedWeightedNum = compared.m_number + (compared.m_number <= 2)  * 13;

    // compare number
    if ( weightedNum < comparedWeightedNum )            // if smaller number
        return true;
    else if ( weightedNum > comparedWeightedNum )       // if bigger number
        return false;
    else                                                // if same number
        return (m_suit < compared.m_suit);              // if compare the suit
}

void Card::operator=(const Card &anotherCard)
{
    m_suit = anotherCard.m_suit;
    m_number = anotherCard.m_number;
    m_image = anotherCard.m_image;
    b_selected = false;
    b_played = false;
}

void Card::paint(QPainter &painter)
{
    painter.drawPixmap(left(),
                       top(),
                       m_image.scaled(CARD_WIDTH, CARD_HEIGHT, Qt::KeepAspectRatio));
}

bool Card::checkMousePress(const QPoint &pos)
{
    if (pos.x() > left() && pos.x() < right()
        && pos.y() > top() && pos.y() < bottom())
    {
        if (b_selected)
        {
            moveTop(top() + CARD_SELECTED_Y_MOVEMENT);
        }
        else
        {
            moveTop(top() - CARD_SELECTED_Y_MOVEMENT);
        }
        b_selected = !b_selected;
        //qDebug() << descSuit() << descNumber() << b_selected;
        return true;
    }
    return false;
}

