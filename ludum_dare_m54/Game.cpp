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
	ShaderHandler::Load("spriteEx", "shaders/spriteEx.vertex", "shaders/spriteEx.fragment");
	BGLShader shaderEx = ShaderHandler::Get("spriteEx");

	BGLBatchTextureHandler::InitializeArrayTextures();
	BGLBatchTextureHandler::LoadArrayTexture("spritesheet", "images/sheet_test.png");
	BGLBatchTextureHandler::LoadArrayTexture("background", "images/background-test.png");
	

	SpriteComponent::InitializeSpriteComponent();

	background.AddComponent<SpriteComponent>("background");
	background.scale = glm::vec2(1200, 800);
	background.position = glm::vec2(600, 400);

	player.AddComponent<SpriteComponent>("player");
	player.AddComponent<BehaviorComponent>("player");
	player.scale = glm::vec2(80, 40);
	player.position = glm::vec2(200, 400);


	srand(SDL_GetTicks());

	// create the test level
	for(int32_t i = 0; i < 500; ++i)
	{
		for(int32_t j = 0; j < 20; ++j)
		{
			if(rand()%100 > 10) continue;

			Entity *e = new Entity();
			e->position = glm::vec2(40*i+20, 40*j+20);
			e->scale = glm::vec2(40, 40);
			e->AddComponent<SpriteComponent>("tile");
		}
	}
}

void Game::update(int frameCount, float dt)
{
	BehaviorComponent::Update(dt);
	SpriteComponent::Update(dt);

	float levelSpeed = 50.f;
	SpriteComponent::camera[LEVEL_LAYER].x += levelSpeed * dt;

}

Game::~Game(void)
{
}

void Game::startGame(std::string mapName)
{
	curMap = new Map(mapName, &resourcer);
	//Should also probably create the player entity here
}