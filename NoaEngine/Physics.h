#ifndef NOAENGINE_PHISICS
#include <vector>
#include "NoaMath.h"

//这个是一个物理类，负责模拟物理的运动和一些碰撞
class Physics
{
private:
	Vector<float>* colliderPos;
protected:
	float mass = 1;
	float g = 9.81;
	bool useGravity = true;
	Vector<float> velocity;
protected:
	Physics(Vector<float>* colliderPos);
	
public:
	void PhysicsUpdate(float deltaTime);
};


#endif // !NOAENGINE_PHISICS
