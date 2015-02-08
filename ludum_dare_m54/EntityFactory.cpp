#include "EntityFactory.h"


EntityFactory::EntityFactory(void)
{
}


EntityFactory::~EntityFactory(void)
{
}

void EntityFactory::CreateBasicEntity(const char* jsonBuf, Entity* entity)
{
    rapidjson::Document doc;
    
    doc.Parse(jsonBuf);
    
    if(doc.HasParseError())
    {
        std::cout << "[-] Error parsing entity json file" << std::endl;
        return;
    }

    if(!(doc.HasMember("size") && doc["size"].IsArray() && doc.HasMember("imageName") && doc.HasMember("normalName") && doc.HasMember("spriteSheets")))
    {
        std::cout << "[-] Error parsing entity from JSON, malformed entity data" << std::endl;
        return;
    }

    if(!(doc["size"].Size() == 2))
    {
        std::cout << "[-] Error parsing entity from JSON, malformed size array" << std::endl;
        return;
    }

    //Entity* tmp = new Entity();
    entity->size = glm::vec2(doc["size"][0].GetInt(), doc["size"][1].GetInt());
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc["spriteSheets"].Accept(writer);

    entity->sheetData = GetSpriteSheet(sb.GetString());
    entity->spriteSheet = (*entity->sheetData)[0];
    
    entity->sprite = BGLSpriteEx::Create(doc["imageName"].GetString(), doc["normalName"].GetString(), entity->size[0], entity->size[1], 0, (uint32_t)entity->spriteSheet->frameData.size(), entity->spriteSheet->getFrameArray());

    return;
}

std::vector<SpriteSheet*>* EntityFactory::GetSpriteSheet(const char* jsonBuf)
{
    rapidjson::Document doc;

    doc.Parse(jsonBuf);

    if(doc.HasParseError())
    {
        std::cout << "[-] Error parsing sprite sheet JSON file" << std::endl;
        return NULL;
    }
    /*
    if(!(doc.HasMember("spriteSheets") && doc["spriteSheets"].IsArray()))
    {
        std::cout << "[-] GetSpriteSheet must have a sprite sheet json passed" << std::endl;
        return NULL;
    }
    */
    if(!(doc.IsArray()))
    {
        std::cout << "[-] Sprite sheet wasn't passed a JSON array!" << std::endl;
        return NULL;
    }

    std::vector<SpriteSheet*>* tmpSheets = new std::vector<SpriteSheet*>();
    for(rapidjson::SizeType i = 0; i < doc.Size(); i++)
    {
        SpriteSheet* tmpSheet = new SpriteSheet();

        const rapidjson::Value& sheetData = doc[i];
        
        if(!(sheetData.HasMember("animTag") && sheetData.HasMember("frames") && sheetData["frames"].IsArray()))
        {
            std::cout << "[-] Error parsing sprite sheet JSON file, malformed sprite sheet" << std::endl;
            return NULL;
        }

        tmpSheet->animTag = sheetData["animTag"].GetString();
        //tmpSheet->entityID = sheetData["entityID"].GetString();
        
        for(rapidjson::SizeType j = 0; j < sheetData["frames"].Size(); j++)
        {
            //Create BGL Rects for the sprite sheet
            if(!(sheetData["frames"][j].IsArray() && sheetData["frames"][j].Size() ==4))
            {
                std::cout << "[-] Error parsing sprite sheet JSON file, malformed frame data" << std::endl;
                return NULL;
            }
            int x = sheetData["frames"][j][0].GetInt();
            int y = sheetData["frames"][j][1].GetInt();
            int w = sheetData["frames"][j][2].GetInt();
            int h = sheetData["frames"][j][3].GetInt();
            BGLRect tmpRect = BGLRectMake((GLfloat)x, (GLfloat)y, (GLfloat)w, (GLfloat)h);
            tmpSheet->frameData.push_back(tmpRect);
        }
        tmpSheets->push_back(tmpSheet);
    }
    return tmpSheets;
}

Player* EntityFactory::CreatePlayerEntity(const char* jsonBuf)
{
    //Call basic stuff, then do whatever is relevant to Player specifically
    Player* tmp = new Player();
    
    CreateBasicEntity(jsonBuf, tmp);

    return tmp;
}

Enemy* EntityFactory::CreateEnemyEntity(const char* jsonBuf)
{
    //Call basic stuff, then do whatever is relevant to Enemy specifically
    Enemy* tmp = new Enemy();

    CreateBasicEntity(jsonBuf, tmp);

    return tmp;

}

Tile* EntityFactory::CreateTileEntity(const char* jsonBuf)
{
    //Call basic stuff, then do whatever is relevant to Tile specifically
    Tile* tmp = new Tile();

    CreateBasicEntity(jsonBuf, tmp);

    return tmp;
}