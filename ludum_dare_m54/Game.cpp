#include "Game.h"
#include "Particles.h"

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
	BGLBatchTextureHandler::LoadArrayTexture("background", "images/background-long.png");
	BGLBatchTextureHandler::LoadArrayTexture("foreground", "images/moon.png");
	BGLBatchTextureHandler::LoadArrayTexture("loading", "images/loading.png");
	BGLBatchTextureHandler::LoadArrayTexture("startscreen", "images/start_screen.png");
	
	SpriteComponent::InitializeSpriteComponent();
	ParticleHandler::Initialize();
	Entity::Initialize();

	background = Entity::Create(glm::vec2(2048, 400), glm::vec2(4096, 800), glm::vec3());
	background->AddComponent<SpriteComponent>("background");

	Entity *foreground = Entity::Create(glm::vec2(1024, 400), glm::vec2(4096, 800), glm::vec3());
	foreground->AddComponent<SpriteComponent>("foreground");


	player = Entity::Create(glm::vec2(200, 400), glm::vec2(80, 40), glm::vec3());
	player->AddComponent<SpriteComponent>("player");
	player->AddComponent<BehaviorComponent>("player");
	runningTime = 0.0f;

	srand(SDL_GetTicks());

	working = true;
	std::cout << "--- Creating the level ---" << std::endl;
	th = std::thread([this] () -> void {
		// create the test level
		//for(int32_t i = 0; i < 1000; ++i)
		//{
		//	for(int32_t j = 0; j < 20; ++j)
		//	{
		//		if(rand()%100 > 3) continue;

		//		Entity *e = new Entity();
		//		e->position = glm::vec2(40*i+20, 40*j+20);
		//		e->scale = glm::vec2(40, 40);
		//		e->AddComponent<SpriteComponent>("tileback");

		//	}
		//}

		//for(int32_t i = 0; i < 1000; ++i)
		//{
		//	for(int32_t j = 0; j < 20; ++j)
		//	{
		//		if(rand()%100 > 3) continue;

		//		Entity *e = new Entity();
		//		e->position = glm::vec2(40*i+20, 40*j+20);
		//		e->scale = glm::vec2(40, 40);
		//		e->AddComponent<SpriteComponent>("tile");

		//	}
		//}

		events.push_back(LevelEvent::CreateAction(50.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(70.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(90.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(110.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(130.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(200.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(240.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(280.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(320.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(360.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));


		events.push_back(LevelEvent::CreateAction(500.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(540.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(580.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(620.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		events.push_back(LevelEvent::CreateAction(660.f, 0.f, 
		([] () -> void {
			Entity *e = Entity::Create("enemy", glm::vec2(1200, 400), glm::vec2(80, 40), glm::vec3());
			e->AddComponent<SpriteComponent>("enemy");
			e->AddComponent<PhysicsComponent>("");
			e->AddComponent<BehaviorComponent>("s-enemy");
		}) ));

		LevelEvent::PrepareLevelEvents(&events);

		// sort the events by time

		working = false;
	});

	BGLSprite loading = BGLSprite::Create(BGLBatchTextureHandler::GetArrayTexture("startscreen"), 1, (BGLRect *)&BGLRectMake(0, 0, 600, 400));
	SpriteComponent::renderLayers[UI_LAYER]->DrawSprite(loading, glm::vec2(600, 400), glm::vec2(1200, 800), glm::vec3(0, 0, 0));

	levelCameraOffset = glm::vec2();
}

void Game::update(int frameCount, float dt)
{
	if(working == true)
	{
		
		SpriteComponent::renderLayers[UI_LAYER]->BeginBatch();
		SpriteComponent::renderLayers[UI_LAYER]->RenderBatch(glm::ortho(0.f, 1200.f, 800.f, 0.f, -1.f, 1.f), glm::mat4());
		//SpriteComponent::renderLayers[UI_LAYER]->ClearBatch();
		return;
	}

	levelCameraOffset.x += 50*dt;

	runningTime += dt*1000;

	// check for LevelEvents to excecute
	while(events.size() > 0)
	{
		if(events.front().offset < levelCameraOffset.x)
		{
			std::cout << "RUNNING TIME: " << runningTime << std::endl;
			events.front().action();
			events.pop_front();
		}
		else
			break;
	}

	BehaviorComponent::Update(dt);
	PhysicsComponent::Update(dt);
	ParticleHandler::Update(dt);
	SpriteComponent::Update(dt);

	SpriteComponent::camera[BACKGROUND_BACKGROUND].x = levelCameraOffset.x/3;
	SpriteComponent::camera[BACKGROUND_FOREGROUND].x = levelCameraOffset.x/6;

	Entity::SweepDestruction();

	//float levelSpeed = 40.f;
	//SpriteComponent::camera[LEVEL_FOREGROUND].x += levelSpeed * dt;
	//SpriteComponent::camera[LEVEL_BACKGROUND].x += levelSpeed * dt * 0.5;

}

Game::~Game(void)
{
}

void Game::startGame(std::string mapName)
{
	curMap = new Map(mapName, &resourcer);
	//Should also probably create the player entity here
}

void Game::cleanup()
{
	Entity::createdEntities.clear();
	Entity::freeEntities.clear();
	Entity::deletedEntities.clear();
}
