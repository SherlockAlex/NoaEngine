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
	class TileCollider2D;

	enum class BodyType {
		STATIC = 0,			//静态物体
		DYNAMIC = 1,		//动态物体
		KINEMATIC = 2		//运动学物体
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
		
		float damping = 0.02f;
		float gravityWeight = 3.5f;
		//bool useMotion = true;
		bool isFrozen = false;
		bool useGravity = true;
		bool useCollision = true;
		Vector<float> velocity = Vector<float>(0.0f, 0.0f);
		float angleVelocity = 0.0f;

	private:

		BodyType bodyType = BodyType::DYNAMIC;

		float mass = 1;
		float invMass = 1;

		//线性速度
		Vector<float> newPosition;
		Vector<float> oldPosition;
		Vector<float> newVelocity;

		//角速度
		
		float newAngleVelocity = 0.0f;

		//动量 = 动量 + 外部冲量 + 玩家输入速度动量
		Vector<float> momentum;
		Vector<float> impuls;

		Vector<float> force = Vector<float>(0.0f, 0.0f);
		std::vector<Collider2D*> colliders;
		Collision collision;

		noa::TileCollider2D* tileCollider2D = nullptr;

		Transform* transform = nullptr;

	private:
		Rigidbody(Actor* actor);
		~Rigidbody();
	public:
		static Rigidbody* Create(Actor* actor);
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

		void ApplyTileFixVelocity();
		void ApplyTileFixPosition();

		void ApplyTileCollision(float deltaTime);

		void BindCollider(Collider2D* collider);

	public:
		Vector<float> GetSumForce();
		Vector<float> GetMomentum();
		void AddForce(const Vector<float> & force, ForceType forceType);
		void SetMass(float value);
		void SetBodyType(BodyType bodyType);

		void AddAntiGravity();

	};

}




#endif // !NOAENGINE_PHISICS
