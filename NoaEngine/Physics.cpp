#include "Physics.h"

using namespace std;
vector<Physics*> physics;

Physics::Physics(Vector<float>* colliderPos):velocity(0.0,0.0)
{
	this->colliderPos = colliderPos;
	physics.push_back(this);
}

//实现物理效果
void Physics::PhysicsUpdate(float deltaTime)
{
	if (useGravity) 
	{
		//如果使用重力
		velocity.y -= g * deltaTime;
	}
}
