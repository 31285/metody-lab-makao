#include <vector>
#include "player.hpp"
#include "card.hpp"
#include "gamestate.hpp"

namespace makao
{

    // Constructors

    makao::Player::Player(bool is_ai)
    {
        this->is_computer = is_ai;
        this->halted_count = 0;
    }

    // Methods

    bool makao::Player::PlayCard(makao::Card *card_to_play, makao::GameState *current_state)
    {
        current_state->SetCardToCurrent(card_to_play);
        return true;
    }
}