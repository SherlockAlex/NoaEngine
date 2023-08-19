#include "Physics.h"
#include "NoaEngine.h"

using namespace std;
vector<Physics*> physics;

Vector<float> sumForce(0.0, 0.0);

float invMass = 1;

Physics::Physics(Vector<float>* colliderPos):velocity(0.0,0.0)
{
	this->colliderPos = colliderPos;
	invMass = 1.0 / mass;
	physics.push_back(this);
}

//实现物理效果
void Physics::PhysicsUpdate(float deltaTime)
{
	if (useGravity) 
	{
		if (!isGrounded)
		{
			//如果使用重力
			velocity.y += 3.5*g * deltaTime;
		}
		else {
			if (velocity.y>0) 
			{
				
				velocity.y = 0;
			}
		}
	}
	
	//处理力和速度的关系
	//F = ma

	velocity = sumForce * deltaTime * invMass + velocity;

	//将速度的量反馈到物体的位移变化
	(*colliderPos) = (*colliderPos) + (velocity * deltaTime);

	///Debug("velocity:( "+to_string(velocity.x)+" , "+to_string(velocity.y)+" )");
	Debug("force:( " + to_string(sumForce.x) + " , " + to_string(sumForce.y) + " )");

}

void Physics::AddForce(Vector<float> force, ForceType forceType)
{
	//添加力到物体上
	switch (forceType)
	{
	case Physics::ContinuousForce:
		//添加恒力到物体上
		sumForce = sumForce + force;
		break;
	case Physics::Impulse:
		//添加一个冲量到物体上，作用完马上就消失
		velocity = force * invMass + velocity;
		break;
	default:
		break;
	}

}
