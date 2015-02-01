#include "SpriteSheet.h"

/**
SpriteSheet::SpriteSheet(std::string animTag, Resourcer* resourcer) : animTag(animTag), resRef(resourcer)
{
    /*
    //We can use this to initialize sprite sheet data, for now make it ugly static
    int sizeX = 64, sizeY = 29;

    for(int i = 0; i < 4; i++)
    {
        //int tmpPosX = i*sizeX; //static width this sheet is tiled vertically
        int tmpPosY = i*sizeY;
        frameVect.push_back(BGLRectMake((GLfloat)0.0f, (GLfloat)tmpPosY, (GLfloat)sizeX, (GLfloat)sizeY));
    }
    //initFramesFromData();

}
**/

SpriteSheet::SpriteSheet(void)
{

}

SpriteSheet::~SpriteSheet(void)
{
}

/*
bool SpriteSheet::initFramesFromData()
{
    char* jsonBuf = (char*)resRef->GetDataFromTag(animTag);
    if(!jsonBuf)
    {
        std::cout << "[-] Error getting data from resourcer" << std::endl;
        return false;
    }

    rapidjson::Document doc;
    doc.Parse(jsonBuf);
    if(doc.HasParseError())
    {
        std::cout << "[-] Couldn't find SpriteSheet in JSON Document" << std::endl;
        return false;
    }

    const rapidjson::Value& spriteObj = doc["SpriteSheet"];

    if(!spriteObj.HasMember("EntityID"))
    {
        std::cout << "[-] EntityID is required in SpriteSheet JSON" << std::endl;
        return false;
    }

    if(!spriteObj.HasMember("AnimTag"))
    {
        std::cout << "[-] AnimTag is required in SpriteSheet JSON" << std::endl;
        return false;
    }

    if(!(spriteObj.HasMember("Frames") && spriteObj["Frames"].IsArray()))
    {
        std::cout << "[-] Frames array (of arrays) is required for SpriteSheet JSON" << std::endl;
        return false;
    }

    entityID = spriteObj["EntityID"].GetString();
    animTag = spriteObj["AnimTag"].GetString();

    const rapidjson::Value& framesArray = spriteObj["Frames"];
    for(rapidjson::SizeType i = 0; i < framesArray.Size(); i++)
    {
        const rapidjson::Value& jsonFD = framesArray[i];
        if(!(jsonFD.IsArray() && jsonFD.Size() == 4))
        {
            std::cout << "[-] Illegal value for Frame data in SpriteSheet Frames array" << std::endl;
            return false;
        }
        frameData.push_back(BGLRectMake((GLfloat)jsonFD[0].GetInt(), (GLfloat)jsonFD[1].GetInt(), (GLfloat)jsonFD[2].GetInt(), (GLfloat)jsonFD[3].GetInt()));
    }

    return true;
}
*/
BGLRect* SpriteSheet::getFrameArray()
{
    return &frameData[0];
}