#include "SpriteSheet.h"


SpriteSheet::SpriteSheet(void)
{
    //We can use this to initialize sprite sheet data, for now make it ugly static
    int sizeX = 64, sizeY = 29;

    for(int i = 0; i < 4; i++)
    {
        //int tmpPosX = i*sizeX; //static width this sheet is tiled vertically
        int tmpPosY = i*sizeY;
        frameVect.push_back(BGLRectMake((GLfloat)0.0f, (GLfloat)tmpPosY, (GLfloat)sizeX, (GLfloat)sizeY));
    }

}


SpriteSheet::~SpriteSheet(void)
{
}

BGLRect* SpriteSheet::getFrameArray()
{
    return &frameVect[0];
}