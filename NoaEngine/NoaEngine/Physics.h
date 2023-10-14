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
		//存储碰撞信息
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
	
	enum class ForceType
	{
		CONTINUOUS_FORCE = 1 << 0,
		IMPULSE_FORCE = 1 << 1,
	};

	class Rigidbody final:public ActorComponent
	{
	private:
		friend class PhysicsSystem;
		friend class Collider2D;
	public:
		float damping = 0.02f;
		float gravityWeight = 3.5;

		bool useMotion = true;
		bool isFrozen = false;
		bool useGravity = true;
		bool useCollision = true;
		bool isGrounded = false;

		

		Vector<float> velocity = Vector<float>(0.0, 0.0);
		

	private:
		float mass = 1;
		float invMass = 1;

		Vector<float> newPosition;
		Vector<float> force = Vector<float>(0.0, 0.0);
		Actor* actor = nullptr;
		TileMap* tileMap = nullptr;
		
		std::vector<Collider2D*> colliders;

		// 存储刚体与人物的碰撞信息
		Collision collision;
		// 设置刚体和TileMap之间的碰撞大小
		Vector<float> tileColliderSacle = Vector<float>(1.0f, 1.0f);

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

		void UpdatePosition();
		//碰撞检测线程
		void ApplyTileCollision();

		void BindCollider(Collider2D* collider);

	public:
		void AddForce(const Vector<float> & force, ForceType forceType);
		void SetTileMap(TileMap * map);
		void SetMass(float value);
		void SetTileColliderScale(float x,float y);

		TileMap* GetTileMap();
		

	};

}




#endif // !NOAENGINE_PHISICS
