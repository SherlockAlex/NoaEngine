#ifndef NOAENGINE_PHISICS
#include <vector>
#include "NoaMath.h"

//�����һ�������࣬����ģ��������˶���һЩ��ײ
class Physics
{
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
};


#endif // !NOAENGINE_PHISICS
