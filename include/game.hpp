#pragma once
#include <vector>
#include "card.hpp"
#include "player.hpp"

namespace makao
{
    class Player;

    class Game
    {
    public:
        std::vector<makao::Card> all_cards;
        std::vector<makao::Player> players;
        std::vector<makao::Card*> deck;
        std::vector<makao::Card*> discarded;
        int turn_counter;


        // Constructors

        Game(int deck_count = 1, int player_count = 4);

        // Methods

        void GeneratePlayingDeck(int deck_count = 1);

        void Shuffle();
    };
}