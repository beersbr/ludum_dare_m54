#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "Entity.h"
#include "Tile.h"
#include "language.h"

//This class loads the resources from files on disk, knows how the data should be structured and loads them appropriately when asked.
class Resourcer
{
public:
    Resourcer(void);
    ~Resourcer(void);
    void LoadResourceFromDisk(String fileName, String tag);
    void* GetDataFromTag(String resourceTag);
private:
    std::vector<String*> loadedTags;
    std::vector<void*> loadedData;
    
};

