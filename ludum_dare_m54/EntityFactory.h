#pragma once

#include <rapidjson\document.h>
#include <rapidjson\reader.h>
#include <rapidjson\writer.h>
#include "SpriteSheet.h"

class EntityFactory
{
public:
    EntityFactory(void);
    ~EntityFactory(void);
    //static Player* CreatePlayerEntity(const char* jsonBuf);
    //static Enemy* CreateEnemyEntity(const char* jsonBuf);
    static Tile* CreateTileEntity(const char* jsonBuf);
private:
    static std::vector<SpriteSheet*>* EntityFactory::GetSpriteSheet(const char* jsonBuf);
    static void CreateBasicEntity(const char* jsonBuf, Entity* entity);
};

