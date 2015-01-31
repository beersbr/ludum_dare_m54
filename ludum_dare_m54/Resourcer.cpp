#include "Resourcer.h"


Resourcer::Resourcer(void)
{
}


Resourcer::~Resourcer(void)
{
}

void Resourcer::LoadResourceFromDisk(String fileName, String tag)
{
    std::ifstream* tmpStream = new std::ifstream(fileName);
    if(tmpStream->is_open())
    {
        //Keep this file open and place it into loaded data, we might need to change this behavior later
        loadedTags.push_back(new String(tag));
        loadedData.push_back(tmpStream);
        return;
    }
    else
    {
        std::cout << "[-] Resourcer: Couldn't Open resource file " << fileName << std::endl;
        return;
    }
}

void* Resourcer::GetDataFromTag(String resourceTag)
{

    std::vector<String*>::iterator iter = std::find(loadedTags.begin(), loadedTags.end(), resourceTag);
    
    if(iter == loadedTags.end())
    {
        std::cout << "[-] Resourcer: Error trying to load resource " << resourceTag << std::endl;
        return (void*)0;
    }

    return loadedData[iter - loadedTags.begin];
}