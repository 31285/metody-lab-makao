#include <iostream>
#include <vector>
#include <ctime>
#include <optional>
#include <algorithm>
#include <sstream>
#include <cctype>
#include "gamestate.hpp"
#include "card.hpp"

namespace makao
{
    class Player;

    enum makao::GameState::ATTACK_TYPE : int
    {
        NONE,
        DRAW,
        HALT,
        REQUIRE
    };

    // Constructors

    makao::GameState::GameState(makao::Game *root)
    {
        srand(time(NULL));

        this->attack_strength = 0;
        this->current_attack = makao::GameState::ATTACK_TYPE::NONE;
        this->root_object = root;
        this->card_played_this_round = false;
        this->color_override = std::nullopt;

        this->GetFirstCard();

        return;
    }

    // Methods

    void makao::GameState::SetCardToCurrent(makao::Card *new_card)
    {
        this->root_object->discarded.push_back(this->current_card);
        this->current_card = new_card;
        return;
    }

    bool makao::GameState::IsDigit(std::string &str)
    {
        for (char c : str)
        {
            if (isdigit(c) == false)
            {
                return false;
            }
        }
        return true;
    }

    void makao::GameState::PrintUI()
    {

        makao::Player *player = &this->root_object->players[(this->root_object->turn_counter - 1) % this->root_object->players.size()];

        std::vector<makao::Card *> accepted_hand = this->GetPlayableCards();

        std::cout << "Your hand:" << std::endl;
        std::cout << "[ ";
        for (makao::Card *card : player->hand)
        {
            std::cout << card->getIndicator() << "  ";
        }
        std::cout << "]" << std::endl;

        // Current state
        std::cout << "Current card: " << this->current_card->getIndicator() << std::endl;

        // Color override
        if (this->color_override != std::nullopt)
        {

            std::string req_color_label;

            if (this->color_override == makao::Card::COLOR::HEARTS)
            {
                req_color_label = "Hearts (♥️ )";
            }
            if (this->color_override == makao::Card::COLOR::DIAMONDS)
            {
                req_color_label = "Diamonds (♦️ )";
            }
            if (this->color_override == makao::Card::COLOR::CLUBS)
            {
                req_color_label = "Clubs (♣️ )";
            }
            if (this->color_override == makao::Card::COLOR::SPADES)
            {
                req_color_label = "Spades (♠️ )";
            }

            std::cout << "Current color: " << req_color_label << std::endl;
        }

        // Choices
        std::cout << "Choose Action:" << std::endl;

        std::cout << "[end] End Turn" << std::endl;
        for (int i = 0; i < accepted_hand.size(); i++)
        {
            std::cout << "[play " << i << "] Play " << accepted_hand[i]->getIndicator() << std::endl;
        }

        // Choice loop
        std::string choice;
        std::vector<std::string> choice_vector;
        bool correct = false;
        while (true)
        {

            std::string temp;

            choice_vector.clear();

            std::getline(std::cin, choice);

            std::stringstream ss(choice);

            while (std::getline(ss, temp, ' '))
            {
                choice_vector.push_back(temp);
            }

            // command execution
            if (choice_vector[0] == "end")
            {
                this->end = true;
                break;
            }
            if ((choice_vector[0] == "play" and choice_vector.size() == 2) and
                (IsDigit(choice_vector[1]) == true) and
                (std::stoi(choice_vector[1]) >= 0 and std::stoi(choice_vector[1]) < accepted_hand.size()))
            {
                this->PlayCard(accepted_hand[std::stoi(choice_vector[1])]);
                player->hand.erase(std::remove(player->hand.begin(), player->hand.end(), accepted_hand[std::stoi(choice_vector[1])]), player->hand.end());
                break;
            }
            std::cout << "Invalid Command!" << std::endl;
        }

        return;
    }

    void makao::GameState::GetFirstCard()
    {
        bool is_card_accepted = false;
        while (is_card_accepted == false)
        {
            this->SetCardToCurrent(this->root_object->deck.back());
            this->root_object->deck.pop_back();
            if (
                this->current_card->shape != makao::Card::SHAPE::TWO and
                this->current_card->shape != makao::Card::SHAPE::THREE and
                this->current_card->shape != makao::Card::SHAPE::FOUR and
                this->current_card->shape != makao::Card::SHAPE::JACK and
                this->current_card->shape != makao::Card::SHAPE::QUEEN and
                this->current_card->shape != makao::Card::SHAPE::KING and
                this->current_card->shape != makao::Card::SHAPE::ACE)
            {
                is_card_accepted = true;
            }
        }
        return;
    }

    makao::Card::SHAPE makao::GameState::GetRequestedShape(bool is_ai)
    {
        this->turns_since_requested = 0;

        if (is_ai == true)
        {
            int card_requested = (rand() % 6) + 5;
            std::string req_card_label;

            switch (card_requested)
            {
            case 5:
                req_card_label = "a Five";
                break;
            case 6:
                req_card_label = "a Six";
                break;
            case 7:
                req_card_label = "a Seven";
                break;
            case 8:
                req_card_label = "an Eight";
                break;
            case 9:
                req_card_label = "a Nine";
                break;
            case 10:
                req_card_label = "a Ten";
                break;
            }

            std::cout << "Player " << ((this->root_object->turn_counter - 1) % this->root_object->players.size() + 1) << " requests " << req_card_label << std::endl;
            return static_cast<makao::Card::SHAPE>(card_requested);
        }
        else
        {
            // Players UI
            std::cout << "Choose the shape you want to request:\n";
            std::cout << "[5] 5\n";
            std::cout << "[6] 6\n";
            std::cout << "[7] 7\n";
            std::cout << "[8] 8\n";
            std::cout << "[9] 9\n";
            std::cout << "[10] 10\n";

            std::string choice;
            std::vector<std::string> choice_vector;

            while (true)
            {

                std::string temp;

                choice_vector.clear();

                std::getline(std::cin, choice);

                std::stringstream ss(choice);

                while (std::getline(ss, temp, ' '))
                {
                    choice_vector.push_back(temp);
                }

                // command execution
                if (choice_vector.size() == 1)
                {
                    if (choice_vector[0] == "5")
                    {
                        return makao::Card::SHAPE::FIVE;
                    }
                    else if (choice_vector[0] == "6")
                    {
                        return makao::Card::SHAPE::SIX;
                    }
                    else if (choice_vector[0] == "7")
                    {
                        return makao::Card::SHAPE::SEVEN;
                    }
                    else if (choice_vector[0] == "8")
                    {
                        return makao::Card::SHAPE::EIGHT;
                    }
                    else if (choice_vector[0] == "9")
                    {
                        return makao::Card::SHAPE::NINE;
                    }
                    else if (choice_vector[0] == "10")
                    {
                        return makao::Card::SHAPE::TEN;
                    }
                    else
                    {
                        std::cout << "Invalid Command" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid Command" << std::endl;
                }
            }
        }
    }

    makao::Card::COLOR makao::GameState::GetRequestedColor(bool is_ai)
    {
        if (is_ai == true)
        {
            int color_requested = (rand() % 4);
            std::string req_color_label;

            switch (color_requested)
            {
            case 0:
                req_color_label = "Hearts (♥️ )";
                break;
            case 1:
                req_color_label = "Diamonds (♦️ )";
                break;
            case 2:
                req_color_label = "Clubs (♣️ )";
                break;
            case 3:
                req_color_label = "Spades (♠️ )";
                break;
            }

            std::cout << "Player " << ((this->root_object->turn_counter - 1) % this->root_object->players.size()) + 1 << " Changed color to " << req_color_label << std::endl;
            return static_cast<makao::Card::COLOR>(color_requested);
        }
        else
        {
            // Players UI
            std::cout << "Choose the color you want to change to:\n";
            std::cout << "[0] Hearts (♥️ )\n";
            std::cout << "[1] Diamonds (♦️ )\n";
            std::cout << "[2] Clubs (♣️ )\n";
            std::cout << "[3] Spades (♠️ )\n";

            std::string choice;
            std::vector<std::string> choice_vector;

            while (true)
            {

                std::string temp;

                choice_vector.clear();

                std::getline(std::cin, choice);

                std::stringstream ss(choice);

                while (std::getline(ss, temp, ' '))
                {
                    choice_vector.push_back(temp);
                }

                // command execution
                if (choice_vector.size() == 1)
                {
                    if (choice_vector[0] == "0")
                    {
                        return makao::Card::COLOR::HEARTS;
                    }
                    else if (choice_vector[0] == "1")
                    {
                        return makao::Card::COLOR::DIAMONDS;
                    }
                    else if (choice_vector[0] == "2")
                    {
                        return makao::Card::COLOR::CLUBS;
                    }
                    else if (choice_vector[0] == "3")
                    {
                        return makao::Card::COLOR::SPADES;
                    }
                    else
                    {
                        std::cout << "Invalid Command" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid Command" << std::endl;
                }
            }
        }
    }
}

void makao::GameState::PlayCard(makao::Card *played_card)
{
    // Reset Color Override
    this->color_override.reset();

    this->card_played_this_round = true;

    // Evaluate card
    switch (played_card->shape)
    {
    // Draw attacks
    case makao::Card::SHAPE::TWO:
        this->current_attack = makao::GameState::ATTACK_TYPE::DRAW;
        this->attack_strength += 2;
        break;
    case makao::Card::SHAPE::THREE:
        this->current_attack = makao::GameState::ATTACK_TYPE::DRAW;
        this->attack_strength += 3;
        break;
    case makao::Card::SHAPE::KING:
        this->current_attack = makao::GameState::ATTACK_TYPE::DRAW;
        this->attack_strength += 5;
        break;

    // Halt attacks;
    case makao::Card::SHAPE::FOUR:
        this->current_attack = makao::GameState::ATTACK_TYPE::HALT;
        this->attack_strength += 1;
        break;

    // Require attack
    case makao::Card::SHAPE::JACK:
        this->current_attack = makao::GameState::ATTACK_TYPE::REQUIRE;
        this->required_card = this->GetRequestedShape(this->root_object->players[(this->root_object->turn_counter - 1) % this->root_object->players.size()].is_computer);
        break;

    // Change Color
    case makao::Card::SHAPE::ACE:
        this->current_attack = makao::GameState::ATTACK_TYPE::NONE;
        this->color_override = this->GetRequestedColor(this->root_object->players[(this->root_object->turn_counter - 1) % this->root_object->players.size()].is_computer);
        break;

    // Other Cards
    default:
        this->attack_strength = 0;
        break;
    }

    std::cout << "Player " << ((this->root_object->turn_counter - 1) % this->root_object->players.size()) + 1 << " played " << played_card->getIndicator() << std::endl;

    this->root_object->discarded.push_back(this->current_card);
    this->current_card = played_card;

    return;
}

std::vector<makao::Card *> makao::GameState::GetPlayableCards()
{
    std::vector<makao::Card *> card_vector;

    makao::Player *player = &this->root_object->players[(this->root_object->turn_counter - 1) % this->root_object->players.size()];
    // Iterates over the player's hand
    for (makao::Card *card : player->hand)
    {
        if (this->card_played_this_round == false)
        {

            switch (this->current_attack)
            {
            case makao::GameState::ATTACK_TYPE::DRAW:
                // Check Shape
                if (
                    card->shape == makao::Card::SHAPE::TWO or
                    card->shape == makao::Card::SHAPE::THREE or
                    card->shape == makao::Card::SHAPE::KING)
                {
                    // Check color
                    if ((this->color_override != std::nullopt and card->color == this->color_override) or
                        (this->color_override == std::nullopt and card->color == this->current_card->color))
                    {
                        card_vector.push_back(card);
                    }
                }
                break;

            case makao::GameState::ATTACK_TYPE::REQUIRE:
                // Check Shape
                if (
                    card->shape == this->required_card or
                    card->shape == makao::Card::SHAPE::JACK)
                {
                    card_vector.push_back(card);
                }
                break;

            case makao::GameState::ATTACK_TYPE::HALT:
                // Check Shape
                if (card->shape == makao::Card::SHAPE::FOUR)
                {
                    card_vector.push_back(card);
                }
                break;

            case makao::GameState::ATTACK_TYPE::NONE:
                if (this->current_card->shape == makao::Card::SHAPE::QUEEN or
                    (this->current_card->shape == makao::Card::SHAPE::ACE and card->color == this->color_override))
                {
                    card_vector.push_back(card);
                }
                else
                {
                    if (card->shape == this->current_card->shape or
                        card->color == this->current_card->color or
                        card->shape == makao::Card::SHAPE::QUEEN or
                        card->shape == makao::Card::SHAPE::ACE)
                    {
                        card_vector.push_back(card);
                    }
                }
                break;
            }
        }
        else
        {
            if (card->shape == this->current_card->shape)
            {
                card_vector.push_back(card);
            }
        }
    }
    return card_vector;
}

void makao::GameState::StartTurn()
{
    // Reset card counter
    this->card_played_this_round = false;
    // Check/Reset Require Attack
    if (this->turns_since_requested > this->root_object->players.size())
    {
        this->current_attack = makao::GameState::ATTACK_TYPE::NONE;
    }

    this->end = false;

    // Advance turn
    this->root_object->turn_counter++;
    // Draw UI
    std::cout << "--------------------" << std::endl;
    std::cout << "Turn: " << this->root_object->turn_counter << std::endl;
    int player_id = (this->root_object->turn_counter - 1) % this->root_object->players.size();

    // Halted check
    if (this->root_object->players[player_id].halted_count > 0)
    {
        this->card_played_this_round = true;
        std::cout << "Player " << player_id + 1 << " is blocked this round." << std::endl;
        this->EndTurn();
        return;
    }

    if (this->root_object->players[player_id].is_computer == true)
    {
        //  Computer's turn

        makao::Player *player = &this->root_object->players[player_id];

        std::vector<makao::Card *> accepted_hand;

        do
        {

            accepted_hand = this->GetPlayableCards();
            if (accepted_hand.size() > 0)
            {
                int choice = (rand() % accepted_hand.size());

                this->PlayCard(accepted_hand[choice]);
                player->hand.erase(std::remove(player->hand.begin(), player->hand.end(), accepted_hand[choice]), player->hand.end());
            }
        } while (accepted_hand.size() > 0);

        this->EndTurn();

        return;
    }
    else
    {
        // Players turn
        switch (this->current_attack)
        {
        case makao::GameState::ATTACK_TYPE::DRAW:
            std::cout << "You are being attacked! You risk drawing " << this->attack_strength << " cards." << std::endl;
            break;

        case makao::GameState::ATTACK_TYPE::HALT:
            std::cout << "You are being attacked! You risk losing next " << this->attack_strength << " round(s)." << std::endl;
            break;

        case makao::GameState::ATTACK_TYPE::REQUIRE:
            std::cout << "You are being attacked! You risk drawing a card." << std::endl;
            break;

        default:
            break;
        }

        while (this->end == false)
        {
            this->PrintUI();
        }
        this->EndTurn();
        return;
    }
}

void makao::GameState::EndTurn()
{
    makao::Player *player = &this->root_object->players[(this->root_object->turn_counter - 1) % this->root_object->players.size()];

    if (player->hand.size() == 0)
    {
        std::cout << "Player " << ((this->root_object->turn_counter - 1) % this->root_object->players.size()) + 1 << " has won!" << std::endl;
        throw makao::GameState::QuitGame();
        // Yup, this is how the game ends. I really dislike the idea but my semi-structured code would be way too much of a mess if i were to exit it normally.
    }

    if (player->halted_count > 0)
    {
        player->halted_count -= 1;
    }
    else

    {
        switch (this->current_attack)
        {
        case makao::GameState::ATTACK_TYPE::DRAW:
            if (this->card_played_this_round == false)
            {
                this->DrawCards(this->attack_strength);
                this->attack_strength = 0;
                this->current_attack = makao::GameState::ATTACK_TYPE::NONE;
            }
            break;

        case makao::GameState::ATTACK_TYPE::REQUIRE:
            this->turns_since_requested += 1;
            if (this->card_played_this_round == false)
            {
                this->DrawCards();
            }
            break;

        case makao::GameState::ATTACK_TYPE::HALT:
            if (this->card_played_this_round == false)
            {
                player->halted_count = this->attack_strength;
                this->attack_strength = 0;
                this->current_attack = makao::GameState::ATTACK_TYPE::NONE;
            }
            break;

        default:
            if (this->card_played_this_round == false)
            {
                this->DrawCards();
            }
            break;
        }
    }
    return;
}

void makao::GameState::InitiateGameLoop()
{
    while (true)
    {
        this->StartTurn();
    }
}

makao::Card *makao::GameState::GetNextCard()
{
    makao::Card *card_to_return = this->root_object->deck.back();
    this->root_object->deck.pop_back();
    return card_to_return;
}

void makao::GameState::DrawCards(int count)
{
    std::cout << "Player " << ((this->root_object->turn_counter - 1) % this->root_object->players.size()) + 1 << " has drawn " << count << " card(s)" << std::endl;
    makao::Player *player = &this->root_object->players[(this->root_object->turn_counter - 1) % this->root_object->players.size()];
    for (int i = 0; i < count; i++)
    {
        if (this->root_object->deck.size() > 0)
        {
            player->hand.push_back(this->GetNextCard());
        }
        else
        {
            this->root_object->Shuffle();
            i--;
        }
    }
}