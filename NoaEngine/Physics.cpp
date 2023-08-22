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
		//������Ʒ
		auto it = std::find(rigidbodys.begin(), rigidbodys.end(), this);
		if (it != rigidbodys.end())
		{
			rigidbodys.erase(it);
		}

		Debug("rigidbody has been done");

	}

	//ʵ������Ч��
	void Rigidbody::RigidbodyUpdate(float deltaTime)
	{
		if (useGravity)
		{
			if (!isGrounded)
			{
				//���ʹ������
				velocity.y += 3.5 * g * deltaTime;
			}
			else {
				if (velocity.y > 0)
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

		//Debug("velocity:( "+to_string(velocity.x)+" , "+to_string(velocity.y)+" )");
		//Debug("force:( " + to_string(sumForce.x) + " , " + to_string(sumForce.y) + " )");

	}

	void Rigidbody::AddForce(Vector<float> force, ForceType forceType)
	{
		//�������������
		switch (forceType)
		{
		case Rigidbody::ContinuousForce:
			//��Ӻ�����������
			sumForce = sumForce + force;
			break;
		case Rigidbody::Impulse:
			//���һ�������������ϣ����������Ͼ���ʧ
			velocity = force * invMass + velocity;
			break;
		default:
			break;
		}

	}

}

