#pragma once

#include <vector>
#include <rapidjson\document.h>
#include <rapidjson\reader.h>
#include "language.h"
#include "EntityFactory.h"
#include "Tile.h"
#include "Resourcer.h"

//Class representing the Map that the Game object will use to load resources
class Map
{

public:
    Map(std::string mapName, Resourcer* res);
    ~Map(void);
    std::vector<Entity*>* getObjectList();
private:
    Resourcer* res;
    std::string mapName;
    bool loadMapData(std::string mapName); // This will populate the object list and the tileList
    std::vector<Entity*>* entityList;      // All of the objects we need to have in the map
    std::vector<Entity*>* tileList;        // Even though a tile is an entity, we're separating them for convenience
   
};

