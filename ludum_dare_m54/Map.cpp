#include "Map.h"


Map::Map(std::string mapName, Resourcer* res) : res(res)
{
    //Figure out how we want to manage resources and get the name from that
    if(loadMapData(mapName))
    {
        //Great!
    }

    return;
}


Map::~Map(void)
{
    //Destroy all objects we have a handle on
    return;
}

bool Map::loadMapData(std::string mapName)
{
    char* jsonBuf = (char*)res->GetDataFromTag(mapName);
    //Map should have data per monster entity, tiles and whatever.

    if(!jsonBuf)
    {
        std::cout << "[-] Error getting json data from resourcer!" << std::endl;
        return false;
    }

    rapidjson::Document doc;

    doc.Parse(jsonBuf);

    if(doc.HasParseError())
    {
        std::cout << "[-] Error loading map " << mapName << std::endl;
        return false;
    }

    if(!doc.HasMember("mapName"))
    {
        std::cout << "[-] Map JSON requires a map name!" << std::endl;
        return false;
    }

    if(!(doc.HasMember("enemies") && doc["enemies"].IsArray()))
    {
        std::cout << "[-] Map JSON Requires entity array!" << std::endl;
        return false;
    }

    if(!(doc.HasMember("tiles") && doc["tiles"].IsArray()))
    {
        std::cout << "[-] Map JSON Requires tiles array!" << std::endl;
        return false;
    }
    mapName = doc["mapName"].GetString(); // Should be an identifier for a tile map of some sort.

    std::vector<std::string> enemyList; // List of enemies to load
    std::vector<std::string> tileVect;  // List of tiles to load

    for(rapidjson::SizeType i = 0; i < doc["enemies"].Size(); i++)
    {
        enemyList.push_back(doc["enemies"][i].GetString());
    }

    for(rapidjson::SizeType i = 0; i < doc["tiles"].Size(); i++)
    {
        tileVect.push_back(doc["tiles"][i].GetString());
    }

    entityList = new std::vector<Entity*>();
    tileList = new std::vector<Entity*>();

    for(int i = 0; i < enemyList.size(); i++)
    {
        //Ask the entity factory to create the object
        char* monJson = (char*)res->GetDataFromTag(enemyList[i]);
        if(!monJson)
        {
            std::cout << "[-] Error getting json data from resourcer!" << std::endl;
            return false;
        }
        entityList->push_back(EntityFactory::CreateEntity(monJson));
    }

    for(int i = 0; i < tileVect.size(); i++)
    {
        char* tileJson = (char*)res->GetDataFromTag(tileVect[i]);
        if(!tileJson)
        {
            std::cout << "[-] Error getting json from resourcer!" << std::endl;
            return false;
        }
        tileList->push_back(EntityFactory::CreateEntity(tileJson));
    }

    return false;
}