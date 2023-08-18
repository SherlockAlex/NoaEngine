#include "Physics.h"

using namespace std;
vector<Physics*> physics;

Physics::Physics(Vector<float>* colliderPos):velocity(0.0,0.0)
{
	this->colliderPos = colliderPos;
	physics.push_back(this);
}

//ʵ������Ч��
void Physics::PhysicsUpdate(float deltaTime)
{
	if (useGravity) 
	{
		//���ʹ������
		velocity.y -= g * deltaTime;
	}
}
