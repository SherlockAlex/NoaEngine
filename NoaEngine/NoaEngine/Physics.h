#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <iostream>

#include <vector>
#include "NoaMath.h"
#include "Actor.h"

namespace noa {

	class Rigidbody;

	extern float gravityAcceration;

	class Scene;
	class Actor;
	class TileMap;
	class Transform;
	class Collider2D;
	class ActorComponent;
	class TileCollider2D;

	enum class BodyType {
		STATIC = 0,			//静态物体
		DYNAMIC = 1,		//动态物体
		KINEMATIC = 2		//运动学物体,物体碰到它
	};

	enum class ForceType
	{
		CONTINUOUS_FORCE = 1 << 0,
		IMPULSE_FORCE = 1 << 1,
	};

	class Rigidbody final:public ActorComponent
	{
	private:
		ACTOR_COMPONENT(Rigidbody)
		friend class PhysicsSystem;
		friend class Collider2D;
		friend class TileCollider2D;
	private:
		Rigidbody(Actor* actor);
		~Rigidbody();
	public:
		static Rigidbody * Create(Actor* actor);
	private:
		void Start() override;
		void Update() override;

		void InitVelocity(float deltaTime);
		//用户添加速度速度操作
		void ApplyVelocity(float deltaTime);
		//添加动量添加
		void InitAngleVelocity(float deltaTime);

		void InitPosition(float deltaTime);
		//计算位置约束
		void ApplyPositon(float deltaTime);

		void ApplyTileCollision(float deltaTime);

		void ApplyTileConstraint(float deltaTime);

		void BindCollider(Collider2D* collider);

	public:
		Vector<float> GetSumForce();
		Vector<float> GetMomentum();
		void AddForce(const Vector<float> & force, ForceType forceType);
		Vector<bool> GetConstraint();
		void AddAntiGravity();

		//建造者模式
		Rigidbody& SetMass(float value);
		Rigidbody& SetBodyType(BodyType bodyType);
		Rigidbody& SetBounce(float value);
		Rigidbody& SetFriction(float value);
		Rigidbody& SetDamping(float value);
		Rigidbody& SetGravityScale(float value);
				 
		Rigidbody& SetIsFrozen(bool value);
		Rigidbody& SetUseGravity(bool value);
		Rigidbody* Apply();

	public:

		float bounce = 0.0f;
		float friction = 0.0f;
		float damping = 0.02f;
		float gravityScale = 3.5f;

		bool isFrozen = false;
		bool useGravity = true;

		Vector<float> velocity = Vector<float>(0.0f, 0.0f);
		float angleVelocity = 0.0f;

		BodyType bodyType = BodyType::DYNAMIC;

	private:

		float mass = 1;
		float invMass = 1;

		Vector<float> newPosition;
		Vector<float> oldPosition;
		Vector<float> newVelocity;

		float newAngleVelocity = 0.0f;

		Vector<float> momentum;
		Vector<float> impuls;

		Vector<float> force = Vector<float>(0.0f, 0.0f);

		std::vector<Collider2D*> colliders;
		noa::TileCollider2D* tileCollider2D = nullptr;

		Transform* transform = nullptr;

		Vector<bool> constraint = { false,false };
		Vector<bool> nextConstraint = { false,false };
	};

}




#endif // !NOAENGINE_PHISICS
