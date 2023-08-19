#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <vector>
#include "NoaMath.h"

//�����һ�������࣬����ģ��������˶���һЩ��ײ
class Physics
{
public:
	//��������
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
	/// ������ʩ����
	/// </summary>
	/// <param name="force">������ֵ�������������Ϊ����������ֵ��ʾ��������ǳ��������ʾ�����Ĵ�С</param>
	/// <param name="forceType">��������</param>
	void AddForce(Vector<float> force,ForceType forceType);


};


#endif // !NOAENGINE_PHISICS
