#pragma once

#include "language.h"
#include "Map.h"
#include "Player.h"
#include "Resourcer.h"
#include "BGL.h"
#include "BGLController.h"

class Game
{
public:
    Game(void);
    ~Game(void);
    void startGame(String mapName);
    void update(int frameCount, float dt);
    bool isRunning();
private:
    void draw();
    bool running;
    Resourcer resourcer;
    Map* curMap;
    Player* tmpPlayer; //extremely temporary, for testing only;
    
};

