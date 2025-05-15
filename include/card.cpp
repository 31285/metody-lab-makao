#include <vector>
#include <string>
#include "card.hpp"

namespace makao
{

    // Constructors

    makao::Card::Card(makao::Card::SHAPE shape, makao::Card::COLOR color)
    {
        this->shape = shape;
        this->color = color;
    }

    makao::Card::Card(int shape, int color)
    {
        this->shape = static_cast<makao::Card::SHAPE>(shape);
        this->color = static_cast<makao::Card::COLOR>(color);
    }

    // Methods

    std::string makao::Card::getIndicator()
    {
        std::string indicator = "";

        switch (this->shape)
        {
        case makao::Card::SHAPE::JOKER:
            indicator += "🃏";
            break;
        case makao::Card::SHAPE::ACE:
            indicator += "A";
            break;
        case makao::Card::SHAPE::TWO:
            indicator += "2";
            break;
        case makao::Card::SHAPE::THREE:
            indicator += "3";
            break;
        case makao::Card::SHAPE::FOUR:
            indicator += "4";
            break;
        case makao::Card::SHAPE::FIVE:
            indicator += "5";
            break;
        case makao::Card::SHAPE::SIX:
            indicator += "6";
            break;
        case makao::Card::SHAPE::SEVEN:
            indicator += "7";
            break;
        case makao::Card::SHAPE::EIGHT:
            indicator += "8";
            break;
        case makao::Card::SHAPE::NINE:
            indicator += "9";
            break;
        case makao::Card::SHAPE::TEN:
            indicator += "10";
            break;
        case makao::Card::SHAPE::JACK:
            indicator += "J";
            break;
        case makao::Card::SHAPE::QUEEN:
            indicator += "Q";
            break;
        case makao::Card::SHAPE::KING:
            indicator += "K";
            break;
        }

        switch (this->color)
        {
        case makao::Card::COLOR::HEARTS:
            indicator += "♥️";
            break;
        case makao::Card::COLOR::DIAMONDS:
            indicator += "♦️";
            break;
        case makao::Card::COLOR::CLUBS:
            indicator += "♣️";
            break;
        case makao::Card::COLOR::SPADES:
            indicator += "♠️";
            break;
        }
        return indicator;
    }
}