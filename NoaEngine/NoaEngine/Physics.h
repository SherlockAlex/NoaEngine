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
		
		//反弹系数
		float bounce = 0.0f;
		//摩擦系数
		float friction = 0.0f;
		//空气阻尼系数
		float damping = 0.02f;
		//重力权重
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

		//约束
		//一个物体进入约束状态条件
		//和运动边界发生碰撞时
		//和已经进入约束状态的物体发生碰撞，同时自身的动量分量趋近0
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
		void SetMass(float value);
		void SetBodyType(BodyType bodyType);
		Vector<bool> GetConstraint();
		void AddAntiGravity();

	};

}




#endif // !NOAENGINE_PHISICS
