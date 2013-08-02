#include "classGameObjectODEMS.h"

classGameObjectODEMS::classGameObjectODEMS(void)
{
	this->physicsComponent = new classPhysicsComponentODE();
}

classGameObjectODEMS::~classGameObjectODEMS(void)
{
	delete this->physicsComponent;
}
