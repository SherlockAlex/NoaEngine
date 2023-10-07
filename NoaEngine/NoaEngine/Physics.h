#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <iostream>

#include <vector>
#include "NoaMath.h"
#include "Actor.h"

namespace noa {
	//这个是一个物理刚体，负责模拟物理的运动和一些碰撞
	//如果想让固体具有物理效果，只要继承此类

	//Rigidbody只能实例化，没办法继承
	class Rigidbody;


	//存储人物与Tile的碰撞信息
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
	/// <summary>
	/// 刚体类，当不在使用刚体时，或者物品被摧毁是，请使用
	/// </summary>
	
	
	enum class ForceType
	{
		CONTINUOUS_FORCE = 1 << 0,
		IMPULSE_FORCE = 1 << 1,
	};

	class Rigidbody final
	{
	public:
		std::string tag = "default";
		float mass = 1;
		float damping = 0.02f;
		Actor* actor;
		Vector<float> newPosition;
		float gravityWeight = 1.0;
	private:
		
		//用于物体之间的碰撞检测
		int indexInMap = -1;
		TileMap* tileMap = nullptr;
		
		Vector<float> force = Vector<float>(0.0, 0.0);

		float invMass = 1;
		bool active = true;

		std::vector<Collider2D*> colliders;

	public:
		
		bool isRemoved = false;
		bool useMotion = true;
		bool isFrozen = false;

		Vector<float> velocity = Vector<float>(0.0,0.0);

		// 存储刚体与人物的碰撞信息
		Collision collision;
		bool useGravity = true;
		bool useCollision = true;

	private:
		Rigidbody(Actor* actor);
	public:
		~Rigidbody();
		static Rigidbody* Create(Actor* actor);

		void Start();
		void Update(float deltaTime);
		void LateUpdate(float deltaTime);

		void UpdatePosition();

		void Destroy();

		void AddForce(const Vector<float> & force, ForceType forceType);
		
		void SetTileMap(TileMap * map);

		TileMap* GetTileMap();

		//碰撞检测线程
		void ApplyCollision();

		void BindCollider(Collider2D * collider);

		void ApplyTrigger();

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

		int GetIndexInMap() const;

		// 获取哈希值
		size_t GetHashCode() const {
			return id;
		}
		
		void SetActive(bool value);

		bool GetActive();


	private:
		size_t id;
		static size_t nextId;

		// 静态函数，用于获取下一个独一无二的哈希值
		static size_t GetNextId() {
			return nextId++;
		}

	};

}




#endif // !NOAENGINE_PHISICS
