#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include "Entity.h"
#include "Tile.h"
#include "language.h"

//This class loads the resources from files on disk, knows how the data should be structured and loads them appropriately when asked.
class Resourcer
{
public:
    Resourcer(void);
    ~Resourcer(void);
    void LoadResourceFromDisk(std::string fileName, std::string tag);
    void* GetDataFromTag(std::string resourceTag);
private:
    std::vector<std::string*> loadedTags;
    std::vector<void*> loadedData;
    
};

