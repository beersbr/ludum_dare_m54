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

	SpriteComponent::InitializeSpriteComponent();

	BGLBatchTextureHandler::InitializeArrayTextures();
	BGLBatchTextureHandler::LoadArrayTexture("background", "images/background-test.png");
	BGLBatchTextureHandler::LoadArrayTexture("spritesheet", "images/sheet_test.png");

	background.AddComponent<SpriteComponent>("background");
	background.scale = glm::vec2(1200, 800);
	background.position = glm::vec2(600, 400);

}

void Game::update(int frameCount, float dt)
{
	 SpriteComponent::Update(dt);
}

Game::~Game(void)
{
}

void Game::startGame(std::string mapName)
{
	curMap = new Map(mapName, &resourcer);
	//Should also probably create the player entity here
}