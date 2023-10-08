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
		Vector<float> sacle = Vector<float>(0, 0);
		bool isHitCollisionTile = false;
		bool isGrounded = false;
		bool isTrigger = false;
		Rigidbody * other = nullptr;
		int hitTileID = -1;

	}Collision;

	extern float gravityAcceration;

	class Scene;
	class Actor;
	class TileMap;
	class Transform;
	class Collider2D;
	
	
	enum class ForceType
	{
		CONTINUOUS_FORCE = 1 << 0,
		IMPULSE_FORCE = 1 << 1,
	};

	class Rigidbody final
	{
	private:
		friend class Actor;
		friend class PhysicsSystem;
		friend class Collider2D;
	public:
		std::string tag = "default";
		float mass = 1;
		float damping = 0.02f;
		float gravityWeight = 1.0;

		bool isRemoved = false;
		bool useMotion = true;
		bool isFrozen = false;
		bool useGravity = true;
		bool useCollision = true;

		Vector<float> velocity = Vector<float>(0.0, 0.0);
		// 存储刚体与人物的碰撞信息
		Collision collision;

	private:
		float invMass = 1;
		bool active = true;

		Vector<float> newPosition;
		Vector<float> force = Vector<float>(0.0, 0.0);
		Actor* actor = nullptr;
		TileMap* tileMap = nullptr;
		
		std::vector<Collider2D*> colliders;

	private:
		Rigidbody(Actor* actor);
		~Rigidbody();
	public:
		static Rigidbody* Create(Actor* actor);
	private:
		void Start();
		void Update();

		void UpdateVelocity(float deltaTime);
		void UpdatePosition(float deltaTime);

		void UpdatePosition();

		void ApplyTrigger();

		//碰撞检测线程
		void ApplyCollision();

		void BindCollider(Collider2D* collider);

	public:
		void Destroy();
		void AddForce(const Vector<float> & force, ForceType forceType);
		void SetTileMap(TileMap * map);
		void SetActive(bool value);
		bool GetActive();
		TileMap* GetTileMap();

	public:

		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(this->actor);
		}

		template<class T>
		bool TryGetActorAs(T & out)
		{
			T buffer = dynamic_cast<T>(this->actor);
			if (buffer == nullptr)
			{
				return false;
			}
			out = buffer;
			return true;
		}

	};

}




#endif // !NOAENGINE_PHISICS
