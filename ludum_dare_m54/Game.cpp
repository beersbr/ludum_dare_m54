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

    tmpPlayer->sprite = Sprite::Create("masterSprite", "", 64, 29, 0, 4, tmpPlayer->spriteData.getFrameArray());

}

void Game::update(int frameCount, float dt)
{
	float dx = 0.0f;
	float dy = 0.0f;
	float speed = 200.0f;
	BGLInputState GameInput = BGLController::GetInputState();
	if(GameInput.controllerSz > 0)
	{
		dx = GameInput.controllers[0].LX * speed * dt;
		dy = GameInput.controllers[0].LY * speed * dt;
	}

	tmpPlayer->sprite.modelTransform = glm::translate(tmpPlayer->sprite.modelTransform, glm::vec3(dx, dy, 0.0f));

    //fuck it, change the frame ASAP!
    if(!(frameCount % 4))
    {
        tmpPlayer->sprite.SetAnimationFrame(tmpPlayer->sprite.currentFrame+1);
    }

    tmpPlayer->sprite.Render();
    
}