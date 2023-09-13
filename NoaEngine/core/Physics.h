#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <vector>
#include "NoaMath.h"
#include "Behaviour.h"

namespace noa {
	//这个是一个物理刚体，负责模拟物理的运动和一些碰撞
	//如果想让固体具有物理效果，只要继承此类

	//存储人物的碰撞信息
	typedef struct Collision 
	{
		bool isHitCollisionTile = false;
		bool isGrounded = false;
		bool isTrigger = false;
		void* other = nullptr;
		bool collisionInfo[2][2] = { false,false,false,false };
		void Update(bool isHitCollisionTile,bool isGrounded) 
		{
			this->isHitCollisionTile = isHitCollisionTile;
			this->isGrounded = isGrounded;
		}

	}Collision;

	extern float gravityAcceration;

	class TileMap;
	class Transform;
	/// <summary>
	/// 刚体类，当不在使用刚体时，或者物品被摧毁是，请使用RemoveRigidbody
	/// </summary>
	class Rigidbody
	{
	public:
		string tag = "default";

		//力的种类
		enum ForceType
		{
			ContinuousForce = 1 << 0,
			Impulse = 1 << 1,
		};
		//质量
		float mass = 1;
		//阻尼系数
		float damping = 0.02;
		Transform* colliderPos = nullptr;
		Vector<float> newPosition;
		float gravityWeight = 1.0;
	private:
		
		//用于物体之间的碰撞检测
		int indexInMap = -1;
		TileMap* tileMap = nullptr;
		
		Vector<float> sumForce = Vector<float>(0.0, 0.0);

	protected:
		float invMass = 1;
		bool useGravity = true;
		//bool isGrounded = false;
		bool useCollision = true;
		void* gameObject = nullptr;

	public:
		
		bool useMotion = true;
		bool isFrozen = false;
		//刚体的速度
		Vector<float> velocity = Vector<float>(0.0,0.0);
		//刚体之间碰撞体半径
		float radius = 0.5;

		Collision collision;

	protected:
		Rigidbody(Transform* colliderPos);
		~Rigidbody();

	public:
		void Start();
		void Update();

		/// <summary>
		/// 给物体施加力
		/// </summary>
		/// <param name="force">力的数值，如果力的种类为恒力，其数值表示力，如果是冲量，则表示冲量的大小</param>
		/// <param name="forceType">力的类型</param>
		void AddForce(const Vector<float> force, ForceType forceType);
		
		void UpdateMap(TileMap * map);

		//碰撞检测线程
		void ApplyCollision();

		//触发触发器，基于触发对象other以一个void*类型的指针
		virtual void OnTrigger(void* other) {}

		/// <summary>
		/// 返回与之相撞的物品
		/// </summary>
		/// <returns></returns>
		//Rigidbody* GetCollisionRigidbody();

		template<class T>
		T GetGameObjectAs() {
			return (T)this->gameObject;
		}

		//void SetCollisionRigidbody(Rigidbody* rigid);

		int GetIndexInMap() const;

		void RemoveRigidbody() const;

		// 获取哈希值
		size_t GetHashCode() const {
			return id;
		}
		


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
