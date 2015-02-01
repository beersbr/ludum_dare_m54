#include "SpriteSheet.h"


SpriteSheet::SpriteSheet(std::string animTag, Resourcer* resourcer) : animTag(animTag), entityID(-1), resRef(resourcer)
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
    */

}

SpriteSheet::SpriteSheet(void)
{

}

SpriteSheet::~SpriteSheet(void)
{
}

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

    return true;
}

BGLRect* SpriteSheet::getFrameArray()
{
    return &frameData[0];
}