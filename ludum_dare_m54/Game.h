#pragma once

#include <iostream>
#include <fstream>
#include "language.h"
#include "Map.h"
#include "Resourcer.h"
#include "BGL.h"
#include "BGLController.h"
#include "BGLAudio.h"
#include "components.h"

#include <thread>
#include <functional>

struct LevelEvent
{
	float time; // ms
	float offset; //px
	std::function<void()> action;

	static LevelEvent CreateAction(float offset, float time, std::function<void()> fn)
	{
		LevelEvent ev;
		ev.time = time;
		ev.action = fn;
		ev.offset = offset;
		return ev;
	}

	static void PrepareLevelEvents(std::list<LevelEvent> *events)
	{
		events->sort([] (LevelEvent a, LevelEvent b) -> bool {
			return (a.time > b.time);
		});
	}
};

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

	std::thread th;
	bool working;

	Entity *background;
	Entity *player;

	std::list<LevelEvent> events;
	float runningTime;
	glm::vec2 levelCameraOffset;

};



bglinternal void 
createPlayerBullet(glm::vec2 pos, glm::vec2 size)
{
	// NOTE(brett): Entity should provide a pool to add entities to. For now I'm throwing it on the heap

	// NOTE(brett): this would pull from the kinematic :: create_player_bullet tag and just give the data to the
	// appropriate components (drawable, kinematic, collidable, etc)

	//Entity *e = new Entity(false);
	//e->sprite = BGLSpriteEx::Create("spritesheet", "", size.x, size.y, 0, 1, &BGLRectMake(0, 96, 16, 16));
	//e->pos = pos;
	//e->tag = "Bullet";
	//e->AddComponent<PhysicsComponent>("AABB");

}

bglinternal void
createPlayerEnemy(glm::vec2 pos, glm::vec2 size)
{
	//Entity *e = new Entity(false);
	//e->sprite = BGLSpriteEx::Create("spritesheet", "", size.x, size.y, 0, 1, &BGLRectMake(32, 0, 32, 16));
	//e->pos = pos;
	//e->tag = "Enemy";
	//e->AddComponent<BehaviorComponent>("cosine_enemy_behavior");
	//e->AddComponent<PhysicsComponent>("AABB");
}