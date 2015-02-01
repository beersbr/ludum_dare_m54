#pragma once

#include "language.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// NOTE(brett): Dont need these as they cause circular dependencies.
// Since pointer are just boing used we can just forward declare them.
//#include "Entity.h"
//#include "Tile.h"


class Entity;
class Tile;

//This class loads the resources from files on disk, knows how the data should be structured and loads them appropriately when asked.
class Resourcer
{
public:
    Resourcer(void);
    ~Resourcer(void);
    void LoadResourceFromDisk(std::string fileName, std::string tag);
    void* GetDataFromTag(std::string resourceTag);
private:
    std::vector<std::string> loadedTags;
    std::vector<void*> loadedData;
    
};

