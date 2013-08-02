#include "classGameObjectAbstract.h"

classGameObjectAbstract::classGameObjectAbstract(void)
{
	this->physicsComponent = 0;
}

classGameObjectAbstract::classGameObjectAbstract(classPhysicsComponentAbstract * physicsComponent)
{
	this->setPhysicsComponent(physicsComponent);
}

void classGameObjectAbstract::setPhysicsComponent(classPhysicsComponentAbstract * physicsComponent)
{
	if (physicsComponent) {
		this->physicsComponent = physicsComponent;
	}
}

void classGameObjectAbstract::setMoveDirection(Direction direction, bool resetOther)
{
	if (this->physicsComponent) {
		this->physicsComponent->setMoveDirection(direction, resetOther);
	}
}

void classGameObjectAbstract::gameCycle(void)
{
	if (this->physicsComponent) {
		this->physicsComponent->gameCycle();
	}
}

bool classGameObjectAbstract::mouseMove(int x, int y)
{
	if (this->physicsComponent) {
		return this->physicsComponent->mouseMove(x, y);
	}
	return false;
}
classGameObjectAbstract::~classGameObjectAbstract(void)
{
}
