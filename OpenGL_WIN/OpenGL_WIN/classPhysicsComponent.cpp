#include "classPhysicsComponent.h"
namespace MY_NS_PHYSICS_COMPONENT
{
	classPhysicsComponent::classPhysicsComponent(void)
	{
		this->moveForward = false;
		this->moveBackward = false;
		this->moveLeft = false;
		this->moveRight = false;
	}

	void classPhysicsComponent::setMoveDirection(Direction direction, bool resetOther)
	{
		if (resetOther) {
			this->moveForward = false;
			this->moveBackward = false;
			this->moveLeft = false;
			this->moveRight = false;
		}
		switch(direction) {
			case Direction::backward:
				this->moveBackward = true;
				break;
			case Direction::forward:
				this->moveForward = true;
				break;
			case Direction::left:
				this->moveLeft = true;
				break;
			case Direction::right:
				this->moveRight = true;
				break;
			default:
				break;
		}
	}

	void classPhysicsComponent::resetMoveDirection(Direction direction)
	{
		switch(direction) {
			case Direction::backward:
				this->moveBackward = false;
				break;
			case Direction::forward:
				this->moveForward = false;
				break;
			case Direction::left:
				this->moveLeft = false;
				break;
			case Direction::right:
				this->moveRight = false;
				break;
			default:
				this->setMoveDirection(Direction::undefined, true);
				break;
		}
	}


	classPhysicsComponent::~classPhysicsComponent(void)
	{
	}
}