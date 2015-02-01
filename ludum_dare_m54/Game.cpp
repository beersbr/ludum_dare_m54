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

Game::Game()
{
	ShaderHandler::Load("sprite", "shaders/sprite.vertex", "shaders/sprite.fragment");
	BGLShader shader = ShaderHandler::Get("sprite");
	std::cout << "ShaderID: " << shader.id << std::endl;
	glUseProgram(shader.id);

	TextureHandler::Load("diffuse", "./images/diffuse.png");
	TextureHandler::Load("normal", "./images/normal.png");
	TextureHandler::Load("testship", "images/ship_spritesheet.png");
	TextureHandler::Load("spritesheet", "./images/sheet_test.png");

	BGLTexture diffuse = TextureHandler::Get("testship");
	BGLRect erects[] = {
		{0, 0, 64, 29  },
		{0, 29, 64, 29 },
		{0, 58, 64, 29 },
		{0, 87, 64, 29 }
	};

	tileSprites[0] = Sprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(0, 208, 16, 16));
	tileSprites[1] = Sprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(32, 208, 16, 16));
	tileSprites[2] = Sprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(32, 224, 16, 16));
	tileSprites[3] = Sprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(0, 224, 16, 16));
	tileSprites[4] = Sprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(112, 240, 16, 16));

	BGLAudio::LoadAudio("laser", "sounds/laser.wav");

	srand(SDL_GetTicks());

	for(uint32_t y = 0; y < 20; ++y)
	{
		for(uint32_t x = 0; x < 60; ++x)
		{

			map[y][x] = tileSprites[rand()%5];
			map[y][x].modelTransform = glm::translate(glm::mat4(), glm::vec3(x*40.0f+20.0f, y*40.0f+20.0f, 0.0f)); 
		}
	}

	tempPlayer = new Player();
	tempPlayer->sprite = Sprite::Create("testship", "", 80, 40, 0, 4, &erects[0]);

	camera = BGLRectMake(0, 0, 1200, 800);
	shootDelay = 0.15f;
	shootElapsed = shootDelay;
}

void Game::update(int frameCount, float dt)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	float dx = 0.0f;
	float dy = 0.0f;
	float speed = 350.0f;

	float cameraSpeed = 50.0f;

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


	shootElapsed += dt;
	if(GameInput.controllerSz > 0)
	{
		if(GameInput.controllers[0].X.down && shootDelay < shootElapsed)
		{
			BGLAudio::PlayAudio("laser");
			shootElapsed = shootElapsed - shootDelay;
		}
	}

	// we want the player to move with the camera 
	dx += cameraSpeed * dt;
	tempPlayer->sprite.modelTransform = glm::translate(tempPlayer->sprite.modelTransform, glm::vec3(dx, dy, 0.0f));

    //fuck it, change the frame ASAP!
    if(!(frameCount % 4))
    {
        tempPlayer->sprite.SetAnimationFrame(tempPlayer->sprite.currentFrame+1);
    }

	// RENDERING 
	
	camera.x += cameraSpeed * dt;
	SetCamera(glm::vec2(camera.x, camera.y));


	BGLRect cameraRect = BGLRectMake(camera.x, camera.y, camera.w + 40, camera.h);
	for(uint32_t y = 0; y < 20; ++y)
	{
		for(uint32_t x = 0; x < 60; ++x)
		{
			
			BGLRect tileRect = BGLRectMake(x*40, y*40, 40, 40);

			if(BGLRectOverlap(camera, tileRect))
			{
				map[y][x].Render();
			}
			
		}
	}	

    tempPlayer->sprite.Render();
    
}

void Game::loadEntity(std::string entityTag)
{

}

Game::~Game(void)
{
}

void Game::startGame(std::string mapName)
{
    tempPlayer = new Player();
    tempPlayer->spriteSheet = new SpriteSheet("PlayerSheet", &resourcer);
    tempPlayer->sprite = Sprite::Create("masterSprite", "", 64, 29, 0, 4, tempPlayer->spriteSheet->getFrameArray());
}
