#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <iostream>

#include <vector>
#include "NoaMath.h"
#include "Actor.h"

namespace noa {
	//�����һ��������壬����ģ��������˶���һЩ��ײ
	//������ù����������Ч����ֻҪ�̳д���

	//Rigidbodyֻ��ʵ������û�취�̳�
	class Rigidbody;

	// �����Ĳ����ڵ�ṹ��
	typedef struct QuadTreeNode {
		float minX;
		float minY;
		float maxX;
		float maxY;
		std::vector<Rigidbody*> rigidbodies;
		QuadTreeNode* children[4]; // �ӽڵ����飬�����ĸ�����
	}QuadTreeNode;


	//�洢�������ײ��Ϣ
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
	/// �����࣬������ʹ�ø���ʱ��������Ʒ���ݻ��ǣ���ʹ��
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
		
		//��������֮�����ײ���
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

		//��ײ����߳�
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

		// ��ȡ��ϣֵ
		size_t GetHashCode() const {
			return id;
		}
		
		void SetActive(bool value);

		bool GetActive();


	private:
		size_t id;
		static size_t nextId;

		// ��̬���������ڻ�ȡ��һ����һ�޶��Ĺ�ϣֵ
		static size_t GetNextId() {
			return nextId++;
		}

	public:
		bool CollisionWithinRigidbody();

	};


	
	//�ռ��Ĳ���
	extern QuadTreeNode* CreateQuadTreeNode(float minX, float minY, float maxX, float maxY);
	
	extern void PerformCollisionDetection(QuadTreeNode* node);

	extern void InsertRigidbody(QuadTreeNode* node, Rigidbody* rigidbody);

}




#endif // !NOAENGINE_PHISICS
