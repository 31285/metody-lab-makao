#pragma once
#include <vector>
#include "card.hpp"
#include "gamestate.hpp"

namespace makao
{
    class GameState;

    class Player
    {
    public:
        bool is_computer;
        std::vector<makao::Card* > hand;
        int halted_count;

        // Constructors

        Player(bool is_ai = true);

        // Methods

        bool PlayCard(makao::Card *card_to_play, makao::GameState *current_state);
    };
}