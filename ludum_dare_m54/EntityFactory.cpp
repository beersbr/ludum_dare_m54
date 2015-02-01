#include "EntityFactory.h"


EntityFactory::EntityFactory(void)
{
}


EntityFactory::~EntityFactory(void)
{
}

Entity* EntityFactory::CreateEntity(const char* jsonBuf)
{
    rapidjson::Document doc;
    
    doc.Parse(jsonBuf);
    
    if(doc.HasParseError())
    {
        std::cout << "[-] Error parsing entity json file" << std::endl;
        return NULL;
    }

    if(!(doc.HasMember("size") && doc["size"].IsArray() && doc.HasMember("imageName") && doc.HasMember("normalName") && doc.HasMember("spriteSheets")))
    {
        std::cout << "[-] Error parsing entity from JSON, malformed entity data" << std::endl;
        return NULL;
    }

    if(!(doc["size"].Size() == 2))
    {
        std::cout << "[-] Error parsing entity from JSON, malformed size array" << std::endl;
        return NULL;
    }

    Entity* tmp = new Entity();
    tmp->size = glm::vec2(doc["size"][0].GetInt(), doc["size"][1].GetInt());
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc["spriteSheets"].Accept(writer);

    tmp->sheetData = GetSpriteSheet(sb.GetString());
    tmp->spriteSheet = (*tmp->sheetData)[0];
    
    tmp->sprite = Sprite::Create(doc["imageName"].GetString(), doc["normalName"].GetString(), tmp->size[0], tmp->size[1], 0, (uint32_t)tmp->spriteSheet->frameData.size(), tmp->spriteSheet->getFrameArray());

    return tmp;
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