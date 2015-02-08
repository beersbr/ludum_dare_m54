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

	std::cout << "ShaderID: " << shader.id << std::endl;
	glUseProgram(shader.id);

	TextureHandler::Load("diffuse", "./images/diffuse.png");
	TextureHandler::Load("normal", "./images/normal.png");
	TextureHandler::Load("testship", "images/ship_spritesheet.png");
	TextureHandler::Load("spritesheet", "./images/sheet_test.png");
	TextureHandler::Load("background", "./images/background.png");

	BGLTexture diffuse = TextureHandler::Get("testship");
	BGLRect erects[] = {
		{0, 0, 64, 29  },
		{0, 29, 64, 29 },
		{0, 58, 64, 29 },
		{0, 87, 64, 29 }
	};


	backgroundSprite = BGLSprite::Create("background", "", 1201, 801, -2, 1, &BGLRectMake(0, 0, 400, 300));
	backgroundSprite.model = glm::translate(backgroundSprite.model, glm::vec3(600.0, 400.0, 0.0f));


	tileSprites[0] = BGLSprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(0, 208, 16, 16));
	tileSprites[1] = BGLSprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(32, 208, 16, 16));
	tileSprites[2] = BGLSprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(32, 224, 16, 16));
	tileSprites[3] = BGLSprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(0, 224, 16, 16));
	tileSprites[4] = BGLSprite::Create("spritesheet", "", 40, 40, -1, 1, &BGLRectMake(112, 240, 16, 16));
	player = Player();
	player.sprite = BGLSprite::Create("testship", "", 100, 60, 0, 4, &erects[0]);
	player.pos.x = 100.0f;
	player.pos.y = 400.0f;

	BGLAudio::LoadAudio("laser", "sounds/laser.wav");

	srand(SDL_GetTicks());

	// NOTE(brett): this is used for the camera
	float tileWidth = 20.0f;
	float tileHeight = 20.0f;

	mapWidth = tileWidth*120;
	mapHeight = tileHeight*20;

	mapSz = 0;

	for(uint32_t y = 0; y < 20; ++y)
	{
		for(uint32_t x = 0; x < 120; ++x)
		{
			if(!(y < 4 || y > 15)) continue;

			tiles[mapSz] = tileSprites[rand()%5];
			tiles[mapSz].model = glm::translate(glm::mat4(), glm::vec3(x*tileWidth+tileWidth/2.0f, y*tileHeight+tileHeight/2.0f, 0.0f));
			mapSz += 1;
		}
	}

	camera = BGLRectMake(0, 0, 1200, 800);


	mapSz = 0;
	for(uint32_t y = 0; y < 100000; ++y)
	{
		tiles[mapSz] = tileSprites[rand()%5];
		int randX = rand()%1200;
		int randY = rand()%800;
		tiles[mapSz].model = glm::translate(glm::mat4(), glm::vec3(randX*tileWidth+tileWidth/2.0f, randY*tileHeight+tileHeight/2.0f, 0.0f));
		mapSz += 1;
	}

}


void Game::update(int frameCount, float dt)
{

	createPlayerEnemy(glm::vec2(), glm::vec2());

	BGLShader shader = ShaderHandler::Get("sprite");
	glUseProgram(shader.id);
	backgroundSprite.model = glm::translate(glm::mat4(), glm::vec3(600.0, 400.0, 0.0f));
	backgroundSprite.Render();

	BGLShader shaderEx = ShaderHandler::Get("spriteEx");
	batcher.shader = shaderEx;

	batcher.BeginBatch();
	for(uint32_t i = 0; i < 10000; i++)
	{
		int randX = rand()%1200;
		int randY = rand()%800;

		batcher.DrawSprite(&tiles[i], glm::vec2(randX, randY), glm::vec2(10, 10), glm::vec3(0, 0, 0));
	}
	glm::mat4 prj = glm::ortho(0.0f, 1200.0f, 800.0f, 0.0f, -1.0f, 1.0f);
	batcher.RenderBatch(prj, glm::mat4());
	batcher.ClearBatch();

	return;

	float cameraSpeed = 50.0f;

	camera.x += cameraSpeed * dt;
	SetCamera(glm::vec2(camera.x, camera.y));
	BGLRect cameraRect = BGLRectMake(camera.x, camera.y, camera.w + 40, camera.h);

	BGLInputState GameInput = BGLController::GetInputState();


	local_persist float spawnTimer = 0.0f;
	spawnTimer += dt;
	if(/*GameInput.keyboard.keys[SDLK_0].down &&*/ spawnTimer > 0.3f)
	{
		createPlayerEnemy(glm::vec2(camera.x + camera.w, camera.y + camera.h/2.0f), glm::vec2(60.0f, 30.0f));
		spawnTimer = 0.0f;
	}

	player.Update(&GameInput, dt);

	// NOTE(brett): Make sure the player does not leave the current view space
	if(player.pos.x < camera.x)
	{
		player.pos.x += camera.x - player.pos.x;
	}
	else if(player.pos.x > (camera.x + camera.w))
	{
		player.pos.x -= player.pos.x - (camera.x + camera.w);
	}
	if(player.pos.y < camera.y)
	{
		player.pos.y += camera.y - player.pos.y;
	}
	else if(player.pos.y > (camera.y + camera.h))
	{
		player.pos.y -= player.pos.y - (camera.y + camera.h);
	}

	player.pos.x += cameraSpeed*dt;


	// NOTE(brett): some of the systems are atomic and can be performed concurrently. The behavior
	// is a good example of that. Maybe we can do all the behaviors at once?
	// NOTE(brett): this is where we update each component type using the static update function
	PhysicsComponent::Update(dt);
	BehaviorComponent::Update(dt);

	std::list<Entity *>::iterator createdEntityIterator = Entity::createdEntities.begin();
	for( ; createdEntityIterator != Entity::createdEntities.end(); )
	{
		if((*createdEntityIterator)->deleted)
			createdEntityIterator = Entity::createdEntities.erase(createdEntityIterator);
		else
			createdEntityIterator++;
	}

	// TODO(brett): Can proabaly just add deleted objects to a list and maybe add a behavior for OnDelete
	std::list<Entity *>::iterator entityIi = Entity::deletedEntities.begin();

	for( ; entityIi != Entity::deletedEntities.end(); entityIi++)
	{
		if(!(*entityIi)) continue;
		(*entityIi)->CleanupComponents();
		delete (*entityIi);
		(*entityIi) = 0;
	}
	Entity::deletedEntities.clear();

	////////////////////////////////////////////////////
	// RENDERING 
	////////////////////////////////////////////////////

	// NOTE(brett): dont need to clear as we are drawing a big sprite in the background
	//glClear(GL_COLOR_BUFFER_BIT);
	// TODO(brett): Make sure to test the performance of all the glm::functions. If they are taking too long
	// then I can just access the array and manipulate the matrix components directly (should not be an issue
	// for just tranlations)
	backgroundSprite.Render();
	backgroundSprite.model = glm::translate(glm::mat4(), glm::vec3(camera.x + 600.0, camera.y + 400.0, 0.0f));

	for(int32_t i = 0; i < mapSz; ++i)
	{
		glm::vec4 pos = tiles[i].model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		BGLRect tileRect = BGLRectMake(pos.x-20, pos.y-20, 40, 40);

		// NOTE(brett): With large maps the game hardly runs without culling
		if(BGLRectOverlap(camera, tileRect))
		{
			// TODO(brett): Need a fast way to get the world coord rect from a sprite. That way 
			// I can cull objects that are not on the screen but are still "hot" (or alive?)
			tiles[i].Render();
		}

	}


	auto it = Entity::createdEntities.begin();
	for( ; it != Entity::createdEntities.end(); ++it)
	{
		//glm::vec4 pos = (*it)->sprite.model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		BGLRect rect = BGLRectMake((*it)->pos.x - 10, (*it)->pos.y - 10, 20, 20);

		if(BGLRectOverlap(camera, rect))
			(*it)->Render();
	}


	player.Render();   
}

Game::~Game(void)
{
}

void Game::startGame(std::string mapName)
{
	curMap = new Map(mapName, &resourcer);
	//Should also probably create the player entity here
}