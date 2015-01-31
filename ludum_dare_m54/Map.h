#pragma once
#include <vector>
#include "language.h"
#include "Entity.h"
#include "Tile.h"

//Class representing the Map that the Game object will use to load resources
class Map
{

public:
    std::vector<Entity*>* getObjectMap();
    Map(String mapName);
    ~Map(void);
private:
    bool loadMapData(String mapName); // This will populate the object list and the tileList
    std::vector<Entity*>* objectList;
    //Maps will have a single tile sprite-sheet and a list of tiles representing which tile it is and where it should be drawn
    std::vector<Tile*>* tileList;
    //Background image data goes here
};

