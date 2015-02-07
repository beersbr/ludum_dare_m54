#pragma once

#include "rapidjson/rapidjson.h"
#include <string>
#include <unordered_map>

class Entity;

class Component
{
public:
	Component(Entity *who)
	{
		owner = who;
	}

	virtual ~Component()
	{
		owner = 0;
	}

	virtual void Initialize(std::unordered_map<std::string, float> args) = 0;
	virtual void Cleanup() = 0;

public:
	Entity *owner;

};