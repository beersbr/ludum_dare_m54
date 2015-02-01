#pragma once

#include <rapidjson\document.h>
#include <rapidjson\reader.h>
#include <rapidjson\writer.h>
#include "SpriteSheet.h"
#include "Entity.h"

class EntityFactory
{
public:
    EntityFactory(void);
    ~EntityFactory(void);
    static std::vector<SpriteSheet*>* EntityFactory::GetSpriteSheet(const char* jsonBuf);
    static Entity* CreateEntity(const char* jsonBuf);
};

