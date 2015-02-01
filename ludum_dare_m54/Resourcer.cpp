#include "Resourcer.h"


Resourcer::Resourcer(void)
{
}


Resourcer::~Resourcer(void)
{
}

void Resourcer::LoadResourceFromDisk(std::string fileName, std::string tag)
{
    std::ifstream* tmpStream = new std::ifstream(fileName, std::ios::in);
    if(tmpStream->is_open())
    {
        //Keep this file open and place it into loaded data, we might need to change this behavior later
        loadedTags.push_back(std::string(tag));

        //Load the data as bytes, prepending it with the length
        tmpStream->seekg(0, tmpStream->end);
        size_t len = tmpStream->tellg();
        char* tmp = new char[len];
        memset(tmp, 0, len);
        tmpStream->seekg(0, tmpStream->beg);

        tmpStream->read(tmp, len);
        tmpStream->close();
        loadedData.push_back(tmp);
        return;
    }
    else
    {
        std::cout << "[-] Resourcer: Couldn't Open resource file " << fileName << std::endl;
        return;
    }
}

void* Resourcer::GetDataFromTag(std::string resourceTag)
{

    std::vector<std::string>::iterator iter = std::find(loadedTags.begin(), loadedTags.end(), resourceTag);
    
    if(iter == loadedTags.end())
    {
        std::cout << "[-] Resourcer: Error trying to load resource " << resourceTag << std::endl;
        return (void*)0;
    }

    return loadedData[iter - loadedTags.begin()];
}