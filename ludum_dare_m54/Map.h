#pragma once

#include <vector>
#include "language.h"
#include "Entity.h"
#include "Tile.h"

//Class representing the Map that the Game object will use to load resources
class Map
{

public:
    Map(std::string mapName);
    ~Map(void);
    std::vector<Entity*>* getObjectList();
private:
    std::string mapName;
    bool loadMapData(std::string mapName); // This will populate the object list and the tileList
    std::vector<Entity*>* EntityList; // All of the objects we need to have in the map
    //Background image data goes here
};

