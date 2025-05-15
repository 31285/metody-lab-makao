#include <iostream>
#include "include/game.hpp"

int main()
{

    try
    {
        makao::Game GameObject = makao::Game();

        makao::GameState GameStateObject = makao::GameState(&GameObject);

        GameStateObject.InitiateGameLoop();
    }
    catch (makao::GameState::QuitGame &e)
    {
        return EXIT_SUCCESS;
    }
}
