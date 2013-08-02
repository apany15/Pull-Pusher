#pragma once
namespace MY_NS_PHYSICS_COMPONENT
{
	enum Direction
	{
		undefined,
		forward,
		backward,
		left,
		right
	};

	class classPhysicsComponent
	{
	protected:
		bool moveForward;
		bool moveBackward;
		bool moveLeft;
		bool moveRight;
	public:
		classPhysicsComponent(void);
		virtual void gameCycle(void) = 0;
		void setMoveDirection(Direction direction, bool resetOther = false);
		void resetMoveDirection(Direction direction = undefined);
		~classPhysicsComponent(void);
	};
}