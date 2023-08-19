#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <vector>
#include "NoaMath.h"

//这个是一个物理类，负责模拟物理的运动和一些碰撞
class Physics
{
public:
	//力的种类
	enum ForceType
	{
		ContinuousForce = 1 << 0,
		Impulse = 1 << 1,
	};

private:
	Vector<float>* colliderPos;
protected:
	float mass = 1;
	float g = 9.81;
	bool useGravity = true;
	bool isGrounded = false;
	Vector<float> velocity;

protected:
	Physics(Vector<float>* colliderPos);
	
public:
	void PhysicsUpdate(float deltaTime);

	/// <summary>
	/// 给物体施加力
	/// </summary>
	/// <param name="force">力的数值，如果力的种类为恒力，其数值表示力，如果是冲量，则表示冲量的大小</param>
	/// <param name="forceType">力的类型</param>
	void AddForce(Vector<float> force,ForceType forceType);


};


#endif // !NOAENGINE_PHISICS
