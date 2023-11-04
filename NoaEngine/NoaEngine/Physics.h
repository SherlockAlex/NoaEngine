#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <iostream>

#include <vector>
#include "NoaMath.h"
#include "Actor.h"

namespace noa {

	class Rigidbody;

	typedef struct Collision 
	{
		//�洢��ײ��Ϣ
		Actor * actor = nullptr;
		bool CompareTag(const std::string & tag) const;

	}Collision;

	extern float gravityAcceration;

	class Scene;
	class Actor;
	class TileMap;
	class Transform;
	class Collider2D;
	class ActorComponent;
	class TileCollider2D;

	enum class BodyType {
		STATIC = 0,			//��̬����
		DYNAMIC = 1,		//��̬����
		KINEMATIC = 2		//�˶�ѧ����
	};

	enum class ForceType
	{
		CONTINUOUS_FORCE = 1 << 0,
		IMPULSE_FORCE = 1 << 1,
	};

	class Rigidbody final:public ActorComponent
	{
	private:
		NOBJECT(Rigidbody)
		friend class PhysicsSystem;
		friend class Collider2D;
		friend class TileCollider2D;
	public:
		
		//����ϵ��
		float bounce = 0.0f;
		//Ħ��ϵ��
		float friction = 0.0f;
		//��������ϵ��
		float damping = 0.02f;
		//����Ȩ��
		float gravityWeight = 3.5f;

		bool isFrozen = false;
		bool useGravity = true;
		bool useCollision = true;
		Vector<float> velocity = Vector<float>(0.0f, 0.0f);
		float angleVelocity = 0.0f;

	private:

		BodyType bodyType = BodyType::DYNAMIC;

		float mass = 1;
		float invMass = 1;

		//�����ٶ�
		Vector<float> newPosition;
		Vector<float> oldPosition;
		Vector<float> newVelocity;

		//���ٶ�
		
		float newAngleVelocity = 0.0f;

		//���� = ���� + �ⲿ���� + ��������ٶȶ���
		Vector<float> momentum;
		Vector<float> impuls;

		Vector<float> force = Vector<float>(0.0f, 0.0f);
		std::vector<Collider2D*> colliders;
		Collision collision;

		noa::TileCollider2D* tileCollider2D = nullptr;

		Transform* transform = nullptr;

		//Լ��
		//һ���������Լ��״̬����
		//���˶��߽緢����ײʱ
		//���Ѿ�����Լ��״̬�����巢����ײ��ͬʱ����Ķ�����������0
		Vector<bool> constraint = {false,false};
		Vector<bool> nextConstraint = {false,false};

	private:
		Rigidbody(Actor* actor);
		~Rigidbody();
	public:
		static Rigidbody* Create(Actor* actor);
	private:
		void Start() override;
		void Update() override;

		void InitVelocity(float deltaTime);
		//�û�����ٶ��ٶȲ���
		void ApplyVelocity(float deltaTime);
		//��Ӷ������
		void InitAngleVelocity(float deltaTime);

		void InitPosition(float deltaTime);
		//����λ��Լ��
		void ApplyPositon(float deltaTime);

		void ApplyTileCollision(float deltaTime);

		void ApplyTileConstraint(float deltaTime);

		void BindCollider(Collider2D* collider);

	public:
		Vector<float> GetSumForce();
		Vector<float> GetMomentum();
		void AddForce(const Vector<float> & force, ForceType forceType);
		void SetMass(float value);
		void SetBodyType(BodyType bodyType);
		Vector<bool> GetConstraint();
		void AddAntiGravity();

	};

}




#endif // !NOAENGINE_PHISICS
