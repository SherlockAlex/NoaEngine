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

//ʵ������Ч��
void Physics::PhysicsUpdate(float deltaTime)
{
	if (useGravity) 
	{
		if (!isGrounded)
		{
			//���ʹ������
			velocity.y += 3.5*g * deltaTime;
		}
		else {
			if (velocity.y>0) 
			{
				
				velocity.y = 0;
			}
		}
	}
	
	//���������ٶȵĹ�ϵ
	//F = ma

	velocity = sumForce * deltaTime * invMass + velocity;

	//���ٶȵ��������������λ�Ʊ仯
	(*colliderPos) = (*colliderPos) + (velocity * deltaTime);

	///Debug("velocity:( "+to_string(velocity.x)+" , "+to_string(velocity.y)+" )");
	Debug("force:( " + to_string(sumForce.x) + " , " + to_string(sumForce.y) + " )");

}

void Physics::AddForce(Vector<float> force, ForceType forceType)
{
	//�������������
	switch (forceType)
	{
	case Physics::ContinuousForce:
		//��Ӻ�����������
		sumForce = sumForce + force;
		break;
	case Physics::Impulse:
		//���һ�������������ϣ����������Ͼ���ʧ
		velocity = force * invMass + velocity;
		break;
	default:
		break;
	}

}
