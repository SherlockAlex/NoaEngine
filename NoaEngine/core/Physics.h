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

	// 定义四叉树节点结构体
	typedef struct QuadTreeNode {
		float minX;
		float minY;
		float maxX;
		float maxY;
		std::vector<Rigidbody*> rigidbodies;
		QuadTreeNode* children[4]; // 子节点数组，代表四个象限
	}QuadTreeNode;


	//存储人物的碰撞信息
	typedef struct Collision 
	{
		Vector<float> sacle = Vector<float>(0, 0);
		float radius = 0.5;
		bool isHitCollisionTile = false;
		bool isGrounded = false;
		bool isTrigger = false;
		Rigidbody * other = nullptr;
		int hitTileID = -1;
		bool collisionInfo[2][2] = { false,false,false,false };
		void Update(bool isHitCollisionTile,bool isGrounded) 
		{
			this->isHitCollisionTile = isHitCollisionTile;
			this->isGrounded = isGrounded;
		}

	}Collision;

	extern float gravityAcceration;

	class Scene;
	class Actor;
	class TileMap;
	class Transform;
	/// <summary>
	/// 刚体类，当不在使用刚体时，或者物品被摧毁是，请使用
	/// </summary>
	class Rigidbody final
	{
	public:
		std::string tag = "default";
		enum ForceType
		{
			ContinuousForce = 1 << 0,
			Impulse = 1 << 1,
		};
		float mass = 1;
		float damping = 0.02;
		Actor* actor;
		Vector<float> newPosition;
		float gravityWeight = 1.0;
	private:
		
		//用于物体之间的碰撞检测
		int indexInMap = -1;
		TileMap* tileMap = nullptr;
		Vector<float> sumForce = Vector<float>(0.0, 0.0);
		float invMass = 1;
		bool active = true;

	public:
		
		bool isRemoved = false;
		bool useMotion = true;
		bool isFrozen = false;

		Vector<float> velocity = Vector<float>(0.0,0.0);
		QuadTreeNode* node;
		Collision collision;
		bool useGravity = true;
		bool useCollision = true;

	private:
		Rigidbody(Actor* actor);
		
		Rigidbody(const Rigidbody&);
		Rigidbody& operator = (const Rigidbody&);

	public:
		~Rigidbody();
		static Rigidbody* Create(Actor* actor);

		void Start();
		void Update();

		void UpdatePosition();

		void Destroy();

		void AddForce(const Vector<float> force, ForceType forceType);
		
		void SetTileMap(TileMap * map);

		TileMap* GetTileMap();

		//碰撞检测线程
		void ApplyCollision();

		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(this->actor);
		}

		template<class T>
		bool TryGetActorAs(T out)
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

	public:
		bool CollisionWithinRigidbody();

	};


	
	//空间四叉树
	extern QuadTreeNode* CreateQuadTreeNode(float minX, float minY, float maxX, float maxY);
	
	extern void PerformCollisionDetection(QuadTreeNode* node);

	extern void InsertRigidbody(QuadTreeNode* node, Rigidbody* rigidbody);

}




#endif // !NOAENGINE_PHISICS
