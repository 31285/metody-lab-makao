#pragma once
#include <optional>
#include <exception>
#include <vector>
#include "card.hpp"
#include "game.hpp"

namespace makao
{
    class Game;

    class GameState
    {
    public:
        enum ATTACK_TYPE : int;

        // Variables
        makao::Card::SHAPE required_card;
        makao::GameState::ATTACK_TYPE current_attack;
        int attack_strength;
        makao::Game *root_object;
        makao::Card *current_card;
        bool card_played_this_round;
        int turns_since_requested;
        std::optional<makao::Card::COLOR> color_override;
        bool end;

        // Quitting
        class QuitGame : public std::exception
        {
        public:
            const char *what() const throw()
            {
                return "Thy should not percieve me";
            }
        };

        // Constructors

        GameState(makao::Game *root);

        // Methods

        void SetCardToCurrent(makao::Card *new_card);

        void GetFirstCard();

        makao::Card::SHAPE GetRequestedShape(bool is_ai = true);

        makao::Card::COLOR GetRequestedColor(bool is_ai = true);

        void PlayCard(makao::Card *played_card);

        void StartTurn();

        bool IsDigit(std::string & str);

        void PrintUI();

        void EndTurn();

        void InitiateGameLoop();

        void DrawCards(int count = 1);

        makao::Card *GetNextCard();

        std::vector<makao::Card *> GetPlayableCards();
    };
}