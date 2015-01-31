#pragma once

#include <vector>
#include "Entity.h"
#include "Tile.h"
#include "language.h"

//This class loads the resources from files on disk, knows how the data should be structured and loads them appropriately when asked.
class Resourcer
{
public:
    Resourcer(void);
    ~Resourcer(void);
    bool LoadObjectDataForMap(String mapName, std::vector<Entity*>* inEntityList);
    
};

