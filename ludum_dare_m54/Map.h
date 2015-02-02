#pragma once

#include <vector>
#include <rapidjson\document.h>
#include <rapidjson\reader.h>
#include "language.h"
#include "EntityFactory.h"
#include "Tile.h"
#include "Resourcer.h"

typedef struct bitmapFileHeader {
    short tbfType;
    long bfSize;
    short bfReserved1;
    short bfReserved2;
    long bfOffBits;
} bmpFileHeader;

typedef struct bitmapInfoHeader {
    long biSize;
    long biWidth;
    long biHeight;
    short biPlanes;
    short biBitCount;
    long  biCompression;
    long biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    long biClrUsed;
    long biClrImportant;
} bmpInfoHeader;

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
    void parseBMP(std::string mapName);
    std::vector<Entity*>* entityList;      // All of the objects we need to have in the map
    std::vector<Entity*>* tileList;        // Even though a tile is an entity, we're separating them for convenience
   
};

