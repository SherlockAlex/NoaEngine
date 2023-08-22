#include "Physics.h"
#include "NoaEngine.h"

using namespace std;

namespace noa {
	vector<Rigidbody*> rigidbodys;

	Vector<float> sumForce(0.0, 0.0);

	float invMass = 1;

	Rigidbody::Rigidbody(Vector<float>* colliderPos) :velocity(0.0, 0.0)
	{
		this->colliderPos = colliderPos;
		invMass = 1.0 / mass;
		rigidbodys.push_back(this);
	}

	Rigidbody::~Rigidbody()
	{
		//销毁物品
		auto it = std::find(rigidbodys.begin(), rigidbodys.end(), this);
		if (it != rigidbodys.end())
		{
			rigidbodys.erase(it);
		}

		Debug("rigidbody has been done");

	}

	//实现物理效果
	void Rigidbody::RigidbodyUpdate(float deltaTime)
	{
		if (useGravity)
		{
			if (!isGrounded)
			{
				//如果使用重力
				velocity.y += 3.5 * g * deltaTime;
			}
			else {
				if (velocity.y > 0)
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

		//Debug("velocity:( "+to_string(velocity.x)+" , "+to_string(velocity.y)+" )");
		//Debug("force:( " + to_string(sumForce.x) + " , " + to_string(sumForce.y) + " )");

	}

	void Rigidbody::AddForce(Vector<float> force, ForceType forceType)
	{
		//添加力到物体上
		switch (forceType)
		{
		case Rigidbody::ContinuousForce:
			//添加恒力到物体上
			sumForce = sumForce + force;
			break;
		case Rigidbody::Impulse:
			//添加一个冲量到物体上，作用完马上就消失
			velocity = force * invMass + velocity;
			break;
		default:
			break;
		}

	}

}

