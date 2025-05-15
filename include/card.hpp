#pragma once
#include <vector>
#include <string>

namespace makao{

    class Card
    {
    public:
        // Enums helping with  Card standarisation
        enum COLOR : int
        {
            HEARTS,   // 0
            DIAMONDS, // 1
            CLUBS,    // 2
            SPADES    // 3
        };

        enum SHAPE : int
        {
            JOKER, // 0
            ACE,   // 1
            TWO,   // 2
            THREE, // 3
            FOUR,  // 4
            FIVE,  // 5
            SIX,   // 6
            SEVEN, // 7
            EIGHT, // 8
            NINE,  // 9
            TEN,   // 10
            JACK,  // 11
            QUEEN, // 12
            KING   // 13
        };

        // Variables

        Card::COLOR color;
        Card::SHAPE shape;

        // Constructors

        Card(Card::SHAPE shape, Card::COLOR color);

        Card(int shape, int color);

        // Methods

        std::string getIndicator();
    };
}