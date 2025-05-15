#include <ctime>
#include <vector>
#include <algorithm>
#include "game.hpp"
#include "card.hpp"
#include "player.hpp"

namespace makao
{

    // Constructors

    makao::Game::Game(int deck_count, int player_count)
    {
        // Create cards
        this->GeneratePlayingDeck(deck_count);

        // Shuffle deck
        this->Shuffle();

        // Create players
        for (int i = 0; i < player_count; i++)
        {
            this->players.push_back(makao::Player());
        }
        this->players[0].is_computer = false;
        
        //Deal cards to players
        for(int i = 0; i < 5; i++){
            for(int p = 0; p < this->players.size(); p++){
                this->players[p].hand.push_back(this->deck.back());
                this->deck.pop_back();
            }
        }

        //Setting up initial variables:
        this->turn_counter = 0;

        return;
    }

    // Methods

    void makao::Game::GeneratePlayingDeck(int deck_count)
    {
        for (int i = 0; i < deck_count; i++)
        {
            for (int c = 0; c <= 3; c++)
            {
                for (int s = 1; s <= 13; s++)
                {
                    makao::Card temp_card = makao::Card(s, c);
                    this->all_cards.push_back(temp_card);
                }
            }
        }

        for (int i = 0; i < this->all_cards.size(); i++)
        {
            this->deck.push_back(&this->all_cards[i]);
        }
        return;
    }

    void makao::Game::Shuffle()
    {
        //Add discarded cards back to the deck
        while(this->discarded.size() > 0){
            this->deck.push_back(this->discarded.back());
            this->discarded.pop_back();
        }

        // Implementation of Fisher-Yates algorithm : https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
        std::srand(unsigned(std::time(NULL)));
        int deck_size = this->deck.size();
        for (int i = 0; i < deck_size - 1; i++)
        {
            int j = (rand() % (deck_size - i)) + i;
            std::swap(this->deck[i], this->deck[j]);
        }
    }
}
