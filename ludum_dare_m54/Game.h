#pragma once

#include "language.h"
#include "Map.h"

class Game
{
public:
    Game(void);
    ~Game(void);
    void startGame(String mapName);
    bool isRunning();
private:
    void update();
    void draw();
    bool running;
    Map* curMap;
    
};

