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
		int leftUpTile = -1;
		int leftDownTile = -1;
		int rightUpTile = -1;
		int rightDownTile =-1;

		void Update(int leftUp,int leftDown,int rightUp,int rightDown) 
		{
			this->leftUpTile = leftUp;
			this->leftDownTile = leftDown;
			this->rightUpTile = rightUp;
			this->rightDownTile = rightDown;
		}

	}Collision;

	class Transform;
	class Rigidbody
	{
	public:
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
	private:
		
		//用于物体之间的碰撞检测
		int indexInMap = -1;
		Rigidbody* collisionRigid;
	protected:
		float invMass = 1;

		float g = 9.81;
		bool useGravity = true;
		bool isGrounded = false;
		bool useCollision = true;
		
		void* gameObject = nullptr;

	public:
		bool isFrozen = false;
		//刚体的速度
		Vector<float> velocity = Vector<float>(0.0,0.0);
		//刚体的碰撞的大小
		Vector<float> colliderSize = Vector<float>(0.0, 0.0);

		Collision collision;
		bool isTrigger = false;
		bool isHitWall = false;

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
		void SetCollisionTileID(std::vector<int> collisionTileIDs);
		void UpdateMap(void * map);
		float FixPosition();
		void UpdateCollision(const Vector<float> nextPosition);

		/// <summary>
		/// 返回与之相撞的物品
		/// </summary>
		/// <returns></returns>
		Rigidbody* GetCollisionRigidbody();

		template<class T>
		T GetGameObjectAs() {
			return (T)this->gameObject;
		}

		void SetCollisionRigidbody(Rigidbody* rigid);

		int GetIndexInMap() const;

	};
}




#endif // !NOAENGINE_PHISICS
