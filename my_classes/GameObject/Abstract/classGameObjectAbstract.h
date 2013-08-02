#pragma once
#include "classPhysicsComponentAbstract.h"
using namespace MY_NS_PHYSICS_COMPONENT;
class classGameObjectAbstract
{
protected:
	classPhysicsComponentAbstract * physicsComponent;
	classGameObjectAbstract(void);
public:
	classGameObjectAbstract(classPhysicsComponentAbstract * physicsComponent);
	void setPhysicsComponent(classPhysicsComponentAbstract * physicsComponent);
	void setMoveDirection(Direction direction, bool resetOther = false);
	bool mouseMove(int x, int y);
	void gameCycle(void);
	
	~classGameObjectAbstract(void);
};
