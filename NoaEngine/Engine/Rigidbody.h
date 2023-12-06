#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <iostream>

#include <vector>
#include "NoaMath.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "Core.h"

namespace noa {

	class NOA_API Rigidbody;

	extern float gravityAcceration;

	class NOA_API Scene;
	class NOA_API Actor;
	class NOA_API TileMap;
	class NOA_API Transform;
	class NOA_API Collider2D;
	class NOA_API ActorComponent;
	class NOA_API TileCollider2D;

	enum class NOA_API BodyType {
		STATIC = 0,			//静态物体
		DYNAMIC = 1,		//动态物体
		KINEMATIC = 2		//运动学物体,物体碰到它
	};

	enum class NOA_API ForceType
	{
		CONTINUOUS_FORCE = 1 << 0,
		IMPULSE_FORCE = 1 << 1,
	};

	class NOA_API Rigidbody final:public ActorComponent
	{
	private:
		ACTOR_COMPONENT(Rigidbody)
		friend class PhysicsSystem;
		friend class Collider2D;
		friend class TileCollider2D;
		friend class BoxCollider2D;
	private:
		Rigidbody(Actor* actor);
		~Rigidbody();
	public:
		static Rigidbody * Create(Actor* actor);
	private:
		void Start() override;
		void Update() override;

		void InitVelocity(float deltaTime);
		void ApplyVelocity(float deltaTime);
		void InitPosition(float deltaTime);
		void ApplyPositon(float deltaTime);
		void ApplyTileCollision(float deltaTime);
		void ApplyTileConstraint(float deltaTime);
		void BindCollider(Collider2D* collider);

	public:
		Vector<float> GetSumForce();
		Vector<float> GetMomentum();
		Vector<bool> GetConstraint();

		void AddForce(const Vector<float> & force, ForceType forceType);
		void AddAntiGravity();

		Rigidbody& SetMass(float value);
		Rigidbody& SetBodyType(BodyType bodyType);
		Rigidbody& SetBounce(float value);
		Rigidbody& SetFriction(float value);
		Rigidbody& SetDamping(float value);
		Rigidbody& SetGravityScale(float value);
		Rigidbody& SetAngularScale(float value);
		Rigidbody& SetIsFrozen(bool value);
		Rigidbody& SetUseGravity(bool value);
		Rigidbody* Apply();

	public:

		float bounce = 0.0f;
		float friction = 0.0f;
		float damping = 0.02f;
		float gravityScale = 3.5f;
		float angularVelocityWeight = 1.0f;
		float angularVelocity = 0.0f;

		bool isFrozen = false;
		bool useGravity = true;

		Vector<float> velocity = Vector<float>(0.0f, 0.0f);
		BodyType bodyType = BodyType::DYNAMIC;

	private:

		float mass = 1;
		float invMass = 1;
		float newAngularVelocity = 0.0f;

		Vector<float> newPosition;
		Vector<float> oldPosition;
		Vector<float> newVelocity;
		Vector<float> impuls;
		Vector<float> force;

		std::vector<Collider2D*> colliders;
		noa::TileCollider2D* tileCollider2D = nullptr;

		Transform* transform = nullptr;

		Vector<bool> constraint = { false,false };
		Vector<bool> nextConstraint = { false,false };

		bool sleep = false;

	};

}




#endif // !NOAENGINE_PHISICS
