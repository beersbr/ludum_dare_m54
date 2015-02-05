#include "component.h"

Component::Component(Entity *who)
{
	owner = who;
}

Component::~Component()
{

}