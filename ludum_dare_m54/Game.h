#pragma once

#include <iostream>
#include <fstream>
#include "language.h"
#include "Map.h"
#include "Player.h"
#include "Resourcer.h"
#include "BGL.h"
#include "BGLController.h"
#include "BGLAudio.h"
#include "components.h"

class Game
{
public:
    Game(std::vector<std::pair<std::string, std::string>>& resourceFileList, std::vector<std::pair<std::string, std::string>>& textureFileList);
    Game();
    ~Game(void);
    void startGame(std::string mapName);
    void update(int frameCount, float dt);
    bool isRunning();

private:
    void draw();
    bool running;
    void initPlayer();
    //void loadEntity(std::string entityTag); //map is gonna do all that
    Resourcer resourcer;
    Map* curMap;       //Will tell us what entities to load.
    Player player; //yeah, we actually do need this.

    
	Player *tempPlayer;
	BGLSprite tileSprites[5];

	BGLSprite tiles[100000];
	int32_t mapSz;


	int32_t mapWidth;
	int32_t mapHeight;

	BGLRect camera;
	float cameraMoveSpeed;

	BGLSprite backgroundSprite;

	BGLSpriteBatch batcher;

};



bglinternal void 
createPlayerBullet(glm::vec2 pos, glm::vec2 size)
{
	// NOTE(brett): Entity should provide a pool to add entities to. For now I'm throwing it on the heap

	// NOTE(brett): this would pull from the kinematic :: create_player_bullet tag and just give the data to the
	// appropriate components (drawable, kinematic, collidable, etc)
	Entity *e = new Entity(false);
	e->sprite = BGLSprite::Create("spritesheet", "", size.x, size.y, 0, 1, &BGLRectMake(0, 96, 16, 16));
	e->pos = pos;
	e->tag = "Bullet";
	e->AddComponent<PhysicsComponent>("AABB");

}

bglinternal void
createPlayerEnemy(glm::vec2 pos, glm::vec2 size)
{
	Entity *e = new Entity(false);
	e->sprite = BGLSprite::Create("spritesheet", "", size.x, size.y, 0, 1, &BGLRectMake(32, 0, 32, 16));
	e->pos = pos;
	e->tag = "Enemy";
	e->AddComponent<BehaviorComponent>("");
	e->AddComponent<PhysicsComponent>("AABB");
}