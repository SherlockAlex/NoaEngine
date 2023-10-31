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
		//´æ´¢Åö×²ÐÅÏ¢
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
		float damping = 0.02f;
		float gravityWeight = 3.5f;
		bool useMotion = true;
		bool isFrozen = false;
		bool useGravity = true;
		bool useCollision = true;
		Vector<float> velocity = Vector<float>(0.0f, 0.0f);
		

	private:
		float mass = 1;
		float invMass = 1;

		Vector<float> newPosition;
		Vector<float> force = Vector<float>(0.0f, 0.0f);
		std::vector<Collider2D*> colliders;
		Collision collision;

		noa::TileCollider2D* tileCollider2D = nullptr;

	private:
		Rigidbody(Actor* actor);
		~Rigidbody();
	public:
		static Rigidbody* Create(Actor* actor);
	private:
		void Start() override;
		void Update() override;
		void UpdateVelocity(float deltaTime);
		void UpdatePosition(float deltaTime);
		//Åö×²¼ì²âÏß³Ì
		void ApplyTileCollision();

		void BindCollider(Collider2D* collider);

	public:
		void AddForce(const Vector<float> & force, ForceType forceType);
		void SetMass(float value);

	};

}




#endif // !NOAENGINE_PHISICS
