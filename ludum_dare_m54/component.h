#pragma once

#include "rapidjson/rapidjson.h"
#include <string>

#include <unordered_map>

class Entity;

class Component
{
public:
	Component(Entity *who);
	virtual ~Component();
	Entity *owner;

	std::string family;
	std::string name;

	virtual void Initialize(std::unordered_map<std::string, float> args) = 0;
};