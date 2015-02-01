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
    Player* player; //yeah, we actually do need this.

    
	Player *tempPlayer;
	Sprite tileSprites[5];
	Sprite map[20][60];
	BGLRect camera;

	float shootDelay;
	float shootElapsed;

};

