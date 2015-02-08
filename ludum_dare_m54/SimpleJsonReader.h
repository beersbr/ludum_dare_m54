#pragma once

#include <iostream>
#include <string>
#include "rapidjson/document.h"

static 
rapidjson::Document LoadRapidJsonDocument(std::string filename)
{
	rapidjson::Document dom;
	
	FILE *infile = fopen(filename.c_str(), "r");
	if(!infile)
	{
		std::cout << "Could not load json config file '" << filename << "'" << std::endl;
		return dom;
	}

	fseek(infile, 0, SEEK_END);
	int64_t fileSz = ftell(infile);
	fseek(infile, 0, SEEK_SET);


	char *json = (char *)malloc(sizeof(char) * fileSz);
	memset(json, 0, fileSz);

	fread(json, sizeof(char), fileSz, infile);

	fclose(infile);

	dom.Parse(json);

	return dom;
}