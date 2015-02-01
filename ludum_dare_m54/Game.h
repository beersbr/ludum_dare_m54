#pragma once

#include <iostream>
#include <fstream>
#include "language.h"
#include "Map.h"
#include "Player.h"
#include "Resourcer.h"
#include "BGL.h"
#include "BGLController.h"

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
    void loadEntity(std::string entityTag);
    Resourcer resourcer;
    Map* curMap;       //Will tell us what entities to load.
    Player* imNotATemporaryPlayerAtAllYouSavage; //extremely temporary, for testing only;
    
};

