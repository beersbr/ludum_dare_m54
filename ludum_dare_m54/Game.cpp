#include "Game.h"

Game::Game(std::vector<std::pair<std::string, std::string>>& resourceFileList, std::vector<std::pair<std::string, std::string>>& textureFileList)
{
    running = false;
    //Load the master sprite sheet into the graphcis card
    curMap = new Map("MPButtPlanet"); // This shit is NOT Loadable yet

    for(int i = 0; i < resourceFileList.size(); i++)
    {
        resourcer.LoadResourceFromDisk(resourceFileList[i].second, resourceFileList[i].first);
    }

    for(int i = 0; i < textureFileList.size(); i++)
    {
        TextureHandler::Load(textureFileList[i].first, textureFileList[i].second);
    }

}


Game::~Game(void)
{
}

void Game::startGame(std::string mapName)
{
    tmpPlayer = new Player();
    tmpPlayer->spriteSheet = new SpriteSheet("PlayerSheet", &resourcer);
    tmpPlayer->sprite = Sprite::Create("masterSprite", "", 64, 29, 0, 4, tmpPlayer->spriteSheet->getFrameArray());
}

void Game::update(int frameCount, float dt)
{
	float dx = 0.0f;
	float dy = 0.0f;
	float speed = 200.0f;
	BGLInputState GameInput = BGLController::GetInputState();

	if(GameInput.keyboard.keys[SDLK_w].down)
	{
		dy = speed * dt * -1.0;
	}
	if(GameInput.keyboard.keys[SDLK_s].down)
	{
		dy = speed * dt * 1.0;
	}
	if(GameInput.keyboard.keys[SDLK_a].down)
	{
		dx = speed * dt * -1.0;
	}
	if(GameInput.keyboard.keys[SDLK_d].down)
	{
		dx = speed * dt * 1.0;
	}

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

void Game::loadEntity(std::string entityTag)
{

}