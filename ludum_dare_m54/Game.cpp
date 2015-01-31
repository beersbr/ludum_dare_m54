#include "Game.h"
#include "Player.h"

Game::Game(void)
{
    running = false;
}


Game::~Game(void)
{

}

void Game::startGame(String mapName)
{
    //Load all the map game stuff, set running to true and call update
    //If we fail to load stuff, we should crash fatally.
    
    Player* ship = new Player();

    running = true;
    while(running)
    {
        update();
    }
    
}

void Game::update(void)
{


}