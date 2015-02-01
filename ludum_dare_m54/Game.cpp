#include "Game.h"

Game::Game(std::vector<std::pair<std::string, std::string>>& resourceFileList, std::vector<std::pair<std::string, std::string>>& textureFileList)
{
    running = false;
    //Load the master sprite sheet into the graphcis card

    for(int i = 0; i < resourceFileList.size(); i++)
    {
        resourcer.LoadResourceFromDisk(resourceFileList[i].second, resourceFileList[i].first);
    }

    for(int i = 0; i < textureFileList.size(); i++)
    {
        TextureHandler::Load(textureFileList[i].first, textureFileList[i].second);
    }
}


Game::Game()
{
	ShaderHandler::Load("sprite", "shaders/sprite.vertex", "shaders/sprite.fragment");
	BGLShader shader = ShaderHandler::Get("sprite");
	std::cout << "ShaderID: " << shader.id << std::endl;
	glUseProgram(shader.id);

	TextureHandler::Load("diffuse", "./images/diffuse.png");
	TextureHandler::Load("normal", "./images/normal.png");
	TextureHandler::Load("testship", "images/ship_spritesheet.png");

	BGLTexture diffuse = TextureHandler::Get("testship");

	BGLRect erects[] = {
		{0, 0, 64, 29  },
		{0, 29, 64, 29 },
		{0, 58, 64, 29 },
		{0, 87, 64, 29 }
	};

	player = new Player();
	player->sprite = Sprite::Create("testship", "", 500, 200, 0, 4, &erects[0]);
}

Game::~Game(void)
{
}

void Game::startGame(std::string mapName)
{
    curMap = new Map(mapName, &resourcer);
    //Should also probably create the player entity here
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

    player->sprite.modelTransform = glm::translate(player->sprite.modelTransform, glm::vec3(dx, dy, 0.0f));

    //fuck it, change the frame ASAP!
    if(!(frameCount % 4))
    {
        player->sprite.SetAnimationFrame(player->sprite.currentFrame+1);
    }

    player->sprite.Render();
    
}
