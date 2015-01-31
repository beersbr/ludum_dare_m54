#include "Game.h"

Game::Game(void)
{
    running = false;
    //Load the master sprite sheet into the graphcis card
    TextureHandler::Load("masterSprite", "./images/ship_spritesheet.png");
    curMap = new Map("MPButtPlanet"); // This shit is NOT Loadable yet
}


Game::~Game(void)
{
}

void Game::startGame(String mapName)
{
    //Load all the map game stuff, set running to true and call update
    //If we fail to load stuff, we should crash fatally.
    
    tmpPlayer = new Player();

    tmpPlayer->sprite.Create("masterSprite", "", 100, 100, 0, 4, tmpPlayer->spriteData.getFrameArray());

    running = true;
    while(running)
    {
        update();
    }
    
}

void Game::update(void)
{


}