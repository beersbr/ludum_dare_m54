#ifndef __BEHAVIOR_BASE_H__
#define __BEHAVIOR_BASE_H__

#include <map>
#include <unordered_map>
#include <string>
#include "Entity.h"

#include "rapidjson/document.h"

class Behavior
{
public:

	Behavior()
	{
		actor = 0;
	}

	~Behavior()
	{
		actor = 0;
	}

	// This happens when a behavior is loaded by the system
	virtual void Load(Entity *owner)
	{ 
		actor = owner; 
	}

	// This is run by the behavior component
	virtual void Initialize(rapidjson::Value configObject){}

	// NOTE(brett): we can do things based on events here because entities are tagged
	// NOTE(brett): this function would be called by a collider comopnent in the physics family
	//virtual void OnCollide(Entity *other){}

	virtual void OnCollide(Entity *other){}

	// This is where any startup code from the programmer should go
	// This is called after Load
	virtual void Start(){}

	// This is called each update step of the game
	virtual void Update(float dt){}

	// Before the behavior removed
	virtual void End(){}

public:
	Entity *actor;
};


template <typename T>
Behavior *CreateBehavior()
{
	return new T;
}


typedef std::unordered_map<std::string, Behavior *(*)()> registeredBehaviorType;

struct BehaviorFactoryBase
{
	static Behavior *CreateInstance(std::string s)
	{
		registeredBehaviorType::iterator it = GetRegisteredBehaviors()->find(s);
		if(it == GetRegisteredBehaviors()->end())
			return 0;
		return it->second();
	}

protected:
	static registeredBehaviorType *GetRegisteredBehaviors()
	{
		if(!registeredBehaviors)
		{
			registeredBehaviors = new registeredBehaviorType;
		}
		return registeredBehaviors;
	}

private:
	static registeredBehaviorType *registeredBehaviors;
};

template<typename T>
struct BehaviorRegistery : BehaviorFactoryBase 
{
	BehaviorRegistery(std::string s)
	{
		GetRegisteredBehaviors()->insert(std::make_pair(s, &CreateBehavior<T>));
	}
};

#endif